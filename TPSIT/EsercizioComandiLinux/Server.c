/*
 * Programma creato da Ancora Giovanni, classe 5^Ci a.s. 2024/25
 * Date di scrittura del programma: 10/12/2024
 * Creazione di un server che andr� a comunicare con il client.
 */
 
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <time.h>
#include <arpa/inet.h>

#define PORT 8090
#define TIMEOUT 1 * CLOCKS_PER_SEC

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma);
int invia(int sock, char* richiesta);
int ricevi(int sock, char* buffer);

int main(){
	int sock;
	char buffer[4096];
	
	struct sockaddr_in client, server;	// Dichiaro una sockaddr_in per il client e una per il server. Dentro verranno salvate le informazioni principali.
	
	int sockServer = socket(AF_INET, SOCK_STREAM, 0);	
	if(sockServer == -1){
		perror("\nSERVER: Errore di creazione del socket del server");
		exit(EXIT_FAILURE);
	}
	int opt = 1;
	if(setsockopt(sockServer, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0){
		perror("Fallita la setsockopt()");
		close(sock);
		exit(EXIT_FAILURE);
	}
	puts("\nSERVER: Ho creato il mio socket.");
	
	int conferma = 1;
	server = creaId_dispositivo(8090, "0.0.0.0", &conferma);	// Mettere "0.0.0.0" è come usare INADDR_ANY. Il server accetterà richieste da qualsiasi sua interfaccia.
	if(conferma != 0){
		perror("SERVER: Errore nella creazione della sockaddr_in del server");
	}
	
	if((bind(sockServer, (struct sockaddr*) &server, sizeof(server))) != 0){
		perror("SERVER: Operazione di bind fallita");
		return 0;
	}
	puts("SERVER: Operazione di bind effettuata.");
	
	if(listen(sockServer, 5) != 0){
		perror("SERVER: Operazione di listen fallta\n");
		return 0;
	}
	puts("SERVER: Effettuata la listen... Sono in ascolto...\n");
	
	int lunghezzaStruct_client = sizeof(client);
	int nuovoSocket = accept(sockServer, (struct sockaddr*) &client, &lunghezzaStruct_client);

	if(nuovoSocket == -1){
		perror("SERVER: Operazione di accept fallita");
		return 0;
	}
	puts("SERVER: Operazione di accept effettuata.\n");

	char* menu = "Menu\n";

	if(invia(nuovoSocket, menu) == -1){
		perror("Errore nell'invio del men�");
		exit(EXIT_FAILURE);
	}
	puts("SERVER: Inviata una risposta al client.\n");

	puts("SERVER: Ho chiuso il socket virtuale.\n");
	shutdown(nuovoSocket, 2);


	return 0;
}

int invia(int sock, char* richiesta){
	// La funzione invia prova a inviare la richiesta del dispositivo, che viene passata come stringa nel parametro 'richiesta'.
	// La richiesta viene inviata dal socket passato nel parametro sock.
	
	if (send(sock, richiesta, strlen(richiesta), 0) < 0){	// Se la funzione send restituisce un valore negativo, c'� stato un errore.
    	perror("Impossibile inviare la richiesta");
    	return -1;
	}
}

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma){
	// La funzione restituisce la sockaddr_in del dispositivo associato alla porta e all'ip passati come parametro.
	// Con la sockaddr_in tutte le informazioni del dispositivo sono salvate in maniera standardizzata per la connessione, in una struct.
	// La variabile puntatore chiamata conferma serve perché lì scriveremo se l'operazione è andata a buon fine o meno, attraverso un valore di flag.
	
	struct sockaddr_in disp;	// Dichiaro la struct che verrà restituita
	memset(&disp, 0, sizeof(disp));	// Azzero i contenuti della struct in caso sia stata dichiarata in un'area di memoria precedentemente occupata
	
	// Settaggio delle informazioni di base
	disp.sin_family = AF_INET;	// Settare la famiglia del socket del server a AF_INET
	disp.sin_port = htons((u_short) port);	// Settare la porta. La porta viene trasformata in uno short positivo, e con la funzione htons si garantisce che sia salvato nello standard big endian di rete.
	
	if(inet_aton(ip, &disp.sin_addr)){	// Con inet_aton iene salvato in disp.sin_addr l'ip del dispositivo. Il valore restituito dalla funzione ci permette di controllare se l'ip è stato trovato.
		*conferma = 0;	// Caso in cui si è trovato l'ip e l'operazione è andata a buon fine
		return disp;
	}else{
		*conferma = 1;	// Caso in cui NON si è trovato l'ip e l'operazione NON è andata a buon fine
		return disp;	// In entrambi i casi restituisco la struct. Ma in questo caso, dato che c'è stato un errore, essa non assume valori significativi. Sta a chi ha chiamato la funzione gestire l'errore.
	}
}

int ricevi(int sock, char* buffer){
	// La funzione ricevi permette di salvare nel buffer passato come parametro ciò che l'altro dispositivo ha inviato (in questo caso il server al client).
	// Viene restituito -1 se c'è stato un errore, 0 se è tutto OK.
	
	int start = clock();	// Per fare il controllo di timeout, salvo in start il numero di clock prima di attendere la risposta
	int now = clock();		// Anche in questa salvo il numero di clock attuali. Questa variabile però si aggiornerà per poter salvare il tempo.

    while((now - start) < TIMEOUT){	// Continuo a fare l'operazione di ricevere finché non passa un intervallo di tempo definito nella costante TIMEOUT
        if((recv(sock, buffer, 4096, 0)) > 0){	// Quando la funzinoe recv restituirà un valore maggiore di 0, avrà ricevuto la risposta, quindi posso salvarla nel buffer e chiudere la connessione.
			return 0;
		}
		now = clock();	// Se ancora non ho ricevuto, aggiorno il tempo passato e ripeto l'operazione.
    }
    
    // Se � passato il TIMEOUT e non ho ricevuto niente...
    shutdown(sock, 2);	// Chiudo la connessione
	perror("Nessuna risposta ricevuta");	// Stampo un messaggio di errore
	strcpy(buffer, "Nessuna risposta ricevuta");	// Salvo nel buffer un messaggio di errore
	return -1;	// Restituisco -1 così faccio sapere al programma main che c'è stato un errore.
}
