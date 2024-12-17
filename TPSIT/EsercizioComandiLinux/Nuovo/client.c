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
#define PORT 8080
#define TIMEOUT 5 * CLOCKS_PER_SEC
#define MAX_STRINGA 4096

int riceviStringa(int sock, char* buffer);
void inviaScelta(int s);

int main(){
	char buffer_menu[MAX_STRINGA];
	
	printf("CLIENT: Creazione della struct per individuare il server...\n");
	struct sockaddr_in server;
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons((u_short) PORT);
	
	if(inet_aton(IP, &server.sin_addr) == 0){
		printf("CLIENT: Errore nell'individuare il server.\n");
		return 0;
	}
	
	printf("CLIENT: Server individuato! Ora creo il socket e lo connetto...\n");
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	if(connect(sock, (struct sockaddr*) &server, sizeof(server))){
		printf("CLIENT: ERRORE nella connessione al server con il socket.\n");
	}
	printf("CLIENT: Socket connesso al server.\n");
	
	printf("CLIENT: Ora devo ricevere il menu.");
	if(riceviStringa(sock, buffer_menu) == -1){
		printf("CLIENT: Errore nella ricezione del menu.\n");
		close(sock);
		return 0;
	}
	
	puts(buffer_menu);
	
	int scelta = 9;
	scanf("%d", &scelta);
	
	while(scelta >= 1 && scelta <= 9){
		inviaScelta(scelta);
		printf("CLIENT: Richiesta n.%d inviata al server.\n", scelta);
		
		char buffer_risp[4096];
		riceviStringa(sock, buffer_risp);
		printf("CLIENT: Il server ha risposto...\nRisposta: %s\n", buffer_risp);
		
		puts(buffer_menu);
		scanf("%d", &scelta);
	}
	
	printf("Hai premuto 9. Sei uscito dal loop delle richieste. Chiudo tutto...\n");
	close(sock);
	return 0;
}

int riceviStringa(int sock, char* buffer){
	int start = clock();
	int now = clock();
	
	while((now - start) < TIMEOUT){
		if((recv(sock, buffer, 4096, 0)) > 0){
        	return 0;
    	}
    	now = clock();
    }
    
    close(sock);
    printf("CLIENT: Nessuna risposta ricevuta.\n");
    return -1;
}

void inviaScelta(int s){
	
	char* mess1 = "ls -l";
	char* mess2 = "m2";
	char* mess3 = "m3";
	char* mess4 = "m4";
	char* mess5 = "m5";
	char* mess6 = "m6";
	char* mess7 = "m7";
	char* mess8 = "m8";
	char* mess9 = "FINE";

	switch(s){
		case 1: send(sock, mess1, strlen(mess1), 0); break;
		case 2: send(sock, mess2, strlen(mess2), 0); break;
		case 3: send(sock, mess3, strlen(mess3), 0); break;
		case 4: send(sock, mess4, strlen(mess4), 0); break;
		case 5: send(sock, mess5, strlen(mess5), 0); break;
		case 6: send(sock, mess6, strlen(mess6), 0); break;
		case 7: send(sock, mess7, strlen(mess7), 0); break;
		case 8: send(sock, mess8, strlen(mess8), 0); break;
		case 9: send(sock, mess9, strlen(mess9), 0); break;
		default: send(sock, mess9, strlen(mess9), 0); break;
	}
	return;
}

