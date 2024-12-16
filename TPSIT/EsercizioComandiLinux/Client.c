/*
 * Programma scritto e commentato da Ancora Giovanni, classe 5^Ci a.s. 2024/25
 * Programmazione di un client che dialoga con un server.
 * Programma scritto il: 10/12/2024, 16/12/2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>	

#include <unistd.h>		
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP "127.0.0.1"
#define PORT 8090
#define TIMEOUT 5 * CLOCKS_PER_SEC

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma);
int connetti(int sock, struct sockaddr_in server);
int invia(int sock, char* richiesta);
int ricevi(int sock, char* buffer);
void convertiLinux(int n, char* str);

int main(){
	int sock = 0;
	char menu[4096];
	char richiesta[4096];

	int conferma;		// Questa variabile serve per salvare e controllare il valore modificato in memoria con un puntatore da creaId_dispositivo, per sapere se l'operazione   andata a buon fine
	struct sockaddr_in server = creaId_dispositivo(PORT, IP, &conferma);	// Richiamo la funzione, salvo la sockaddr_in del server nella variabiles server. Serve per sapere tutte le informazioni sul server per la connessione.
	if(conferma != 0){	// Controllo se l'operazione   andata a buon fine
		perror("\nCLIENT: Errore nell'individuazione del socket del server");
		return 0;
	}
	// Se mi trovo qui, le operazioni di creazione della sockaddr_in sono andate a buon fine
	
	sock = socket(AF_INET, SOCK_STREAM, 0);	// Creo il socket di connessione del client, famiglia AF_INET, tipo SOCK_STREAM. Lo 0 serve per utilizzare il protocollo di default per questo tipo di socket.
	puts("\nCLIENT: Ho creato il mio socket.");

	if (connetti(sock, server) == -1){	// Provo a connettermi con la funzione connetti. Se c'  stato un errore, viene restituito -1 e termino.
		return 0;
	}
	puts("CLIENT: Mi sono connesso al server.");

	
	if(ricevi(sock, menu) == -1){
		perror("Errore nella ricezione del menù");
		exit(EXIT_FAILURE);
	}
	puts(menu);
	
	int input;
	printf("\nInserire l'input: ");
	scanf("%d", &input);
	
	while(input >= 1 && input <= 8){
		convertiLinux(input, richiesta);
		printf("Richiesta da inviare: %s\n", richiesta);
		int confInvia = invia(sock, richiesta);
		puts("Inviato.");
		if(confInvia == -1){
			puts("Errore nell'invio della richiesta.\n");
			return 0;
		}
		puts("Richeista inviata.\n");
		
		puts(menu);
		printf("\nInserire l'input: ");
		scanf("%d", &input);
	}
	
	printf("Chiusura della connessione...");
	close(sock);
	return 0;
}

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma){
	// La funzione restituisce la sockaddr_in del dispositivo associato alla porta e all'ip passati come parametro.
	// Con la sockaddr_in tutte le informazioni del dispositivo (il server in questo caso) sono salvate in maniera standardizzata per la connessione, in una struct.
	// La variabile puntatore chiamata conferma serve perch  l  scriveremo se l'operazione   andata a buon fine o meno, attraverso un valore di flag.
	
	struct sockaddr_in disp;	// Dichiaro la struct che verr  restituita
	memset(&disp, 0, sizeof(disp));	// Azzero i contenuti della struct in caso sia stata dichiarata in un'area di memoria precedentemente occupata
	
	// Settaggio delle informazioni di base
	disp.sin_family = AF_INET;	// Settare la famiglia del socket del server a AF_INET
	disp.sin_port = htons((u_short) port);	// Settare la porta. La porta viene trasformata in uno short positivo, e con la funzione htons si garantisce che sia salvato nello standard big endian di rete.
	
	if(inet_aton(ip, &disp.sin_addr)){	// Con inet_aton iene salvato in disp.sin_addr l'ip del dispositivo. Il valore restituito dalla funzione ci permette di controllare se l'ip   stato trovato.
		*conferma = 0;	// Caso in cui si   trovato l'ip e l'operazione   andata a buon fine
		return disp;
	}else{
		*conferma = 1;	// Caso in cui NON si   trovato l'ip e l'operazione NON   andata a buon fine
		return disp;	// In entrambi i casi restituisco la struct. Ma in questo caso, dato che c'  stato un errore, essa non assume valori significativi. Sta a chi ha chiamato la funzione gestire l'errore.
	}
}

int connetti(int sock, struct sockaddr_in server){
	// La funzione prova ad aprire la connessione con il server, le cui informazioni sono salvate nella struct passata come paramerto (chiamata server)
	// In sock passiamo il socket che ha aperto il dispositivo client per la connessione
	// L'intero restituito   -1 se c'  stato un errore.
	
	int conn = connect(sock, (struct sockaddr*)&server, sizeof(server));	// La funzione connect apre la connessione con il server passato come sockadd_in, e il socket locale salvato in sock
	if (conn != 0){
    	perror("Impossibile stabilire la connessione");	// Caso di errore se connetti restituisce 0.
    	return -1;
	}
}

int invia(int sock, char* richiesta){
	// La funzione invia prova a inviare la richiesta del dispositivo, che viene passata come stringa nel parametro 'richiesta'.
	// La richiesta viene inviata dal socket passato nel parametro sock.
	
	if (send(sock, richiesta, strlen(richiesta), 0) < 0){	// Se la funzione send restituisce un valore negativo, c'  stato un errore.
    	perror("Impossibile inviare la richiesta");
    	return -1;
	}
}

int ricevi(int sock, char* buffer){
	// La funzione ricevi permette di salvare nel buffer passato come parametro ci  che l'altro dispositivo ha inviato (in questo caso il server al client).
	// Viene restituito -1 se c'  stato un errore, 0 se   tutto OK.
	
	int start = clock();	// Per fare il controllo di timeout, salvo in start il numero di clock prima di attendere la risposta
	int now = clock();		// Anche in questa salvo il numero di clock attuali. Questa variabile per  si aggiorner  per poter salvare il tempo.

    while((now - start) < TIMEOUT){	// Continuo a fare l'operazione di ricevere finch  non passa un intervallo di tempo definito nella costante TIMEOUT
        if((recv(sock, buffer, 4096, 0)) > 0){	// Quando la funzinoe recv restituir  un valore maggiore di 0, avr  ricevuto la risposta, quindi posso salvarla nel buffer e chiudere la connessione.
          shutdown(sock, 2);
          return 0;
        }
		  now = clock();	// Se ancora non ho ricevuto, aggiorno il tempo passato e ripeto l'operazione.
    }
    
    // Se   passato il TIMEOUT e non ho ricevuto niente...
    shutdown(sock, 2);	// Chiudo la connessione
	perror("CLIENT: Nessuna risposta ricevuta dal server (Timeout)");	// Stampo un messaggio di errore
	strcpy(buffer, "Nessuna risposta ricevuta");	// Salvo nel buffer un messaggio di errore
	return -1;	// Restituisco -1 cos  faccio sapere al programma main che c'  stato un errore.
}

void convertiLinux(int n, char* str){
	printf("Numero da convertire: %d\n", n);
	memset(str, 0, sizeof(str));
	switch (n){
		case 1: strcpy(str, "ls -l"); break; 		// Elenca file e cartelle
		case 2: strcpy(str, "a"); break;			// Mostra i socket in ascolto
		case 3: strcpy(str, "b"); break;			// Mostra le interfacce di rete
		case 4: strcpy(str, "c"); break;			// Fai 5 ping al localhost
		case 5: strcpy(str, "d"); break;			// Mostra tutti gli indirizzi IP
		case 6: strcpy(str, "e"); break;			// Mostra solo gli indirizzi IPv4
		case 7: strcpy(str, "f"); break;			// Mostra tutti i processi in esecuzione con tutti i dettagli
		case 8: strcpy(str, "g"); break;			// Mostra tutti i processi in esecuzione con una rappresentazione ad albero
		default: strcpy(str, "FINE");				// Fine della comunicazione
	}
//	puts(str);
	return;
}
