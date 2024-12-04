/*
 * Programma scritto e commentato da Ancora Giovanni, classe 5^Ci a.s. 2024/25
 * Programmazione di un client, che verrà usato per la comunicazione di rete con un server
 */

#include <stdio.h>			// Serve per l'input e output a schermo di dati (printf, puts, perror, ...)
#include <stdlib.h>			// 
#include <string.h>			// 
#include <time.h>			// 

#include <unistd.h>			// 
//#include <sys/wait.h>		// Libreria non necessaria
//#include <signal.h>		// Libreria non necessaria

//#include <netdb.h>		// Libreria non necessaria
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP "172.16.12.54"
#define PORT 80
#define TIMEOUT 1 * CLOCKS_PER_SEC

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma);
int connetti(int sock, struct sockaddr_in server);
int invia(int sock, char* richiesta);
int ricevi(int sock, char* buffer);

int main(){
	int sock = 0;
	char buffer[4096];
	char *richiesta = "GET /~ancora2200/index.html HTTP/1.1\r\nHost: localhost \r\n\r\n";	// Salvo la richiesta come stringa nella variabile richiesta

	int conferma;		// Questa variabile serve per salvare e controllare il valore modificato in memoria con un puntatore da creaId_dispositivo, per sapere se l'operazione è andata a buon fine
	
	struct sockaddr_in server = creaId_dispositivo(PORT, IP, &conferma);	// Richiamo la funzione, salvo la sockaddr_in del server nella variabiles server. Serve per sapere tutte le informazioni sul server per la connessione.
	if(conferma != 0){	// Controllo se l'operazione è andata a buon fine
		perror("Errore nell'individuazione del socket del server");
		return 0;
	}
	// Se mi trovo qui, le operazioni di creazione della sockaddr_in sono andate a buon fine
	
	sock = socket(AF_INET, SOCK_STREAM, 0);	// Creo il socket di connessione del client, famiglia AF_INET, tipo SOCK_STREAM. Lo 0 serve per utilizzare il protocollo di default per questo tipo di socket.

	if (connetti(sock, server) == -1){	// Provo a connettermi con la funzione connetti. Se c'è stato un errore, viene restituito -1 e termino.
		return 0;
	}

	if (invia(sock, richiesta) == -1){	// Operazione analoga alla funzinoe connetti.
		return 0;
	}

	puts("Risposta: \n"); 
	ricevi(sock, buffer);	// Salvo nel buffer la risposta ricevuta
	puts(buffer);			// Stampo il buffer. Se c'è stato un errore, nel buffer c'è salvato un messaggio di errore.
}

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma){
	// La funzione restituisce la sockaddr_in del dispositivo associato alla porta e all'ip passati come parametro.
	// Con la sockaddr_in tutte le informazioni del dispositivo (il server in questo caso) sono salvate in maniera standardizzata per la connessione, in una struct.
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

int connetti(int sock, struct sockaddr_in server){
	// La funzione prova ad aprire la connessione con il server, le cui informazioni sono salvate nella struct passata come paramerto (chiamata server)
	// In sock passiamo il socket che ha aperto il dispositivo client per la connessione
	// L'intero restituito è -1 se c'è stato un errore.
	
	int conn = connect(sock, (struct sockaddr*)&server, sizeof(server));	// La funzione connect apre la connessione con il server passato come sockadd_in, e il socket locale salvato in sock
	if (conn != 0){
    	perror("Impossibile stabilire la connessione");	// Caso di errore se connetti restituisce 0.
    	return -1;
	}
}

int invia(int sock, char* richiesta){
	// La funzione invia prova a inviare la richiesta del dispositivo, che viene passata come stringa nel parametro 'richiesta'.
	// La richiesta viene inviata dal socket passato nel parametro sock.
	
	if (send(sock, richiesta, strlen(richiesta), 0) < 0){	// Se la funzione send restituisce un valore negativo, c'è stato un errore.
    	perror("Impossibile inviare la richiesta");
    	return -1;
	}
}

int ricevi(int sock, char* buffer){
	// La funzione ricevi permette di salvare nel buffer passato come parametro cià che l'altro dispositivo ha inviato (in questo caso il server al client).
	// Viene restituito -1 se c'è stato un errore, 0 se è tutto OK.
	
	int start = clock();	// Per fare il controllo di timeout, salvo in start il numero di clock prima di attendere la risposta
	int now = clock();		// Anche in questa salvo il numero di clock attuali. Questa variabile però si aggiornerà per poter salvare il tempo.

    while((now - start) < TIMEOUT){	// Continuo a fare l'operazione di ricevere finché non passa un intervallo di tempo definito nella costante TIMEOUT
        if((recv(sock, buffer, 4096, 0)) > 0){	// Quando la funzinoe recv restituirà un valore maggiore di 0, avrò ricevuto la risposta, quindi posso salvarla nel buffer e chiudere la connessione.
			shutdown(sock, 2);
			return 0;
		}
		now = clock();	// Se ancora non ho ricevuto, aggiorno il tempo passato e ripeto l'operazione.
    }
    
    // Se è passato il TIMEOUT e non ho ricevuto niente...
    shutdown(sock, 2);	// Chiudo la connessione
	perror("Nessuna risposta ricevuta");	// Stampo un messaggio di errore
	strcpy(buffer, "Nessuna risposta ricevuta");	// Salvo nel buffer un messaggio di errore
	return -1;	// Restituisco -1 così faccio sapere al programma main che c'è stato un errore.
}
