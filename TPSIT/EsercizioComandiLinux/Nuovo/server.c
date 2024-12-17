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


#define SERVER_IP "127.0.0.1"
#define PORT 8090

#define TIMEOUT 5 * CLOCKS_PER_SEC

int main(){
	
	// CREAZIONE STRUCT PER INTERFACCIA INGRESSO SERVER
	struct sockaddr_in server;
	
	memset(&server, 0, sizeof(server));
	server.sin_family = AF_INET;
	server.sin_port = htons(INADDR_ANY);
	
	if(inet_aton(PORT, "0.0.0.0") == 0){
		printf("SERVER: Errore nell'individuazione dell'interfaccia delle richieste.\n");
		return 0;
	}
	
	// CREAZIONE SOCKET INGRESSO SERVER
	int sockServer = socket(AF_INET, SOCK_STREAM, 0);
	if(sockServer == -1){
		printf("SERVER: Errore nella creazione del mio socket.\n");
		return 0;
	}
	
	// Continua con setsockopt.
}
