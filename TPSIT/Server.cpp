/*
 * Programma creato da Ancora Giovanni, classe 5^Ci a.s. 2024/25
 * Date di scrittura del programma: 03/12/2024
 * Creazione di un server che andrà a comunicare con il client.
 */
 
#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT 8080

struct sockaddr_in creaId_dispositivo(int port, char* ip, int* conferma);

int main(){
	int sock;
	char buffer[4096];
	
	struct sockaddr_in client, server;	// Dichiaro una sockaddr_in per il client e una per il server. Dentro verranno salvate le informazioni principali.
	
	int sockServer = socket(AF_INET, SOCK_STREAM, 0);	
	if(sockServer == -1){
		perror("Errore di creazinoe del socket del server\n");
		return 0;
	}
	puts("Socket del server creato.\n");
	
	int conferma = 1;
	server = creaId_dispositivo(8080, INADDR_ANY, &conferma);
	if(conferma != 0){
		perror("Errore nella creazione della sockaddr_in del server\n");
	}
	
	if((bind(sockServer, &server, sizeof(server))) != 0){
		perror("SERVER: Operazione di bind fallita\n")
		return 0;
	}
	
	if(listen(sockServer, 5) != 0){
		perror("SERVER: Operazione di listen fallta\n");
		return 0;
	}
	puts("\nServer in ascolto...\n");
	
	int lunghezzaStruct_client = sizeof(client);
	int confermaConnessione = accept(sockServer, &client, &lunghezzaStruct_client);
	//CONTINUA DA QUI
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
