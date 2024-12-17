#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define SERVER_IP "127.0.0.1"
#define PORT 8080
#define BUFFER_SIZE 1024

void display_menu() {
    printf("\n+--------------------------------------------------+\n");
    printf("|                   Menù Comandi                 |\n");
    printf("+--------------------------------------------------+\n");
    printf("| 1) Elenca file e cartelle (ls -l)                |\n");
    printf("| 2) Mostra i sockets in ascolto sulla macchina   |\n");
    printf("| 3) Visualizza le interfacce di rete presenti     |\n");
    printf("| 4) Fai 5 ping al localhost                       |\n");
    printf("| 5) Mostra tutti gli indirizzi IP                |\n");
    printf("| 6) Mostra solo gli indirizzi IPv4               |\n");
    printf("| 7) Mostra tutti i processi in esecuzione         |\n");
    printf("| 8) Mostra tutti i processi in esecuzione a forma |\n");
    printf("|    di albero (pstree)                           |\n");
    printf("| 9) Fine                                          |\n");
    printf("+--------------------------------------------------+\n");
    printf("Scegli un comando (1-9): ");
}

int main() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    int choice;

    // Creazione del socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Errore nella creazione del socket");
        exit(1);
    }

    // Impostazione dell'indirizzo del server
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);  // Porta 8080
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // Connessione al server
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Errore nella connessione al server");
        exit(1);
    }

    while (1) {
        // Mostra il menù e leggi la scelta
        display_menu();
        scanf("%d", &choice);
        getchar();  // Rimuovi il newline

        // Se il comando è "Fine", chiudi il client
        if (choice == 9) {
            send(sock, "9", 1, 0);
            printf("Uscita...\n");
            break;
        }

        // Invia la scelta al server
        snprintf(buffer, sizeof(buffer), "%d", choice);
        send(sock, buffer, strlen(buffer), 0);

        // Riceve l'output del comando dal server e lo stampa
        printf("Risultato dal server:\n");
        while (1) {
            int bytes_received = recv(sock, buffer, sizeof(buffer) - 1, 0);
            if (bytes_received <= 0) {  // Se bytes_received <= 0, la connessione è chiusa o non ci sono più dati
                break;
            }
            buffer[bytes_received] = '\0';  // Assicurati che la stringa sia correttamente terminata
            printf("%s", buffer);
        }
        printf("\n");
    }

    // Chiudi il socket
    close(sock);

    return 0;
}
