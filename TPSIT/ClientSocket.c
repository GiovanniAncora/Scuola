#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>

#define IP "172.16.12.54"
#define PORT 80
#define TIMEOUT 1 * CLOCKS_PER_SEC

void addr_init(struct sockaddr_in *addr, int port, long int ip);

int main(){
	int sock = 0, i = 0, n = 0;
	char *richiesta = "GET /~ancora2200/index.html HTTP/1.1\r\nHost: localhost \r\n\r\n";
	char buffer[4096];

	struct in_addr a;
	in_addr_t addr;
	addr = inet_addr(IP);

	a.s_addr = addr;
	struct sockaddr_in server;

	sock = socket(AF_INET, SOCK_STREAM, 0);

	memset(&server, 0, sizeof(server));
	addr_init(&server, PORT, addr);

	int conn = connect(sock, (struct sockaddr*)&server, sizeof(server));
	if (conn != 0){
    	puts("Impossibile stabilire una connessione.\n");
    	return 0;
	}

	if (send(sock, richiesta, strlen(richiesta), 0) < 0){
    	puts("Impossibile inviare la richiesta.\n");
    	return 0;
	}

	int start = clock();
	int now = clock();

    while((now - start) < TIMEOUT){
        if((recv(sock, buffer, 4096, 0)) > 0){
            puts("Risposta ricevuta:\n\n");
            puts(buffer);
            puts("\n\n");
            shutdown(sock, 2);
            return 0;
		}
		now = clock();
    }
    puts("Nessuna risposta ricevuta.\n");
    shutdown(sock, 2);
    return 0;
}

void addr_init(struct sockaddr_in *addr, int port, long int ip){
    addr -> sin_family = AF_INET;
    addr -> sin_port = htons((u_short) port);
    addr -> sin_addr.s_addr = INADDR_ANY;
}
