#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void handle_client(int client_sock) {
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while (1) {
        // Riceve il comando dal client
        bytes_received = recv(client_sock, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Errore nella ricezione del comando");
            close(client_sock);
            return;
        }

        buffer[bytes_received] = '\0'; // Null-terminate the string
        int command = atoi(buffer); // Converti il comando in un numero

        char command_output[BUFFER_SIZE];
        FILE *fp;
        
        switch (command) {
            case 1:
                fp = popen("ls -l", "r");
                break;
            case 2:
                fp = popen("ss -l", "r");
                break;
            case 3:
                fp = popen("ip a", "r");
                break;
            case 4:
                fp = popen("ping -c 5 localhost", "r");
                break;
            case 5:
                fp = popen("ip addr show", "r");
                break;
            case 6:
                fp = popen("ip -4 addr", "r");
                break;
            case 7:
                fp = popen("ps aux", "r");
                break;
            case 8:
                fp = popen("pstree", "r");
                break;
            case 9:
                send(client_sock, "Connessione terminata.\n", 23, 0);
                close(client_sock);
                return;
            default:
                send(client_sock, "Comando non valido.\n", 19, 0);
                continue;
        }

        if (fp == NULL) {
            perror("Errore nell'esecuzione del comando");
            send(client_sock, "Errore nell'esecuzione del comando.\n", 36, 0);
            continue;
        }

        // Leggi l'output del comando e invialo al client
        while (fgets(command_output, sizeof(command_output), fp) != NULL) {
            send(client_sock, command_output, strlen(command_output), 0);
        }

        // Chiudiamo il file pointer dopo aver inviato l'output
        fclose(fp);
    }
}

int main() {
    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);

    // Creazione del socket
    if ((server_sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    // Impostazione dell'indirizzo del server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Associazione del socket all'indirizzo
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nel bind");
        exit(1);
    }

    // Inizio ad ascoltare le connessioni
    if (listen(server_sock, 5) < 0) {
        perror("Errore nel listen");
        exit(1);
    }

    printf("Server in ascolto sulla porta %d...\n", PORT);

    // Accetta la connessione dal client
    client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_len);
    if (client_sock < 0) {
        perror("Errore nell'accettare la connessione");
        exit(1);
    }

    printf("Client connesso!\n");

    // Gestisci la comunicazione con il client
    handle_client(client_sock);

    // Chiudi il socket del client e del server
    close(client_sock);
    close(server_sock);

    return 0;
}
