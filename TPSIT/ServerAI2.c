#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

void execute_command(int client_socket, char *command) {
    char buffer[BUFFER_SIZE];
    FILE *fp;

    fp = popen(command, "r");
    if (fp == NULL) {
        perror("popen");
        exit(EXIT_FAILURE);
    }

    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        send(client_socket, buffer, strlen(buffer), 0);
    }

    pclose(fp);
}

int main() {
    int server_fd, client_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *menu = "1) Elenca file e cartelle\n2) Mostra i socket in ascolto sulla macchina\n3) Visualizza le interfacce di rete presenti nel sistema\n4) Fai 5 ping al localhost\n5) Mostra tutti gli indirizzi IP\n6) Mostra solo gli indirizzi IPv4\n7) Mostra tutti i processi in esecuzione con tutti i dettagli\n8) Mostra tutti i processi in esecuzione con una rappresentazione ad albero\n9) Fine\n";

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server in ascolto sulla porta %d\n", PORT);

    if ((client_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    send(client_socket, menu, strlen(menu), 0);

    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        read(client_socket, buffer, BUFFER_SIZE);

        int choice = atoi(buffer);
        switch (choice) {
            case 1:
                execute_command(client_socket, "ls -l");
                break;
            case 2:
                execute_command(client_socket, "netstat -tuln");
                break;
            case 3:
                execute_command(client_socket, "ifconfig");
                break;
            case 4:
                execute_command(client_socket, "ping -c 5 localhost");
                break;
            case 5:
                execute_command(client_socket, "ip addr");
                break;
            case 6:
                execute_command(client_socket, "ip -4 addr");
                break;
            case 7:
                execute_command(client_socket, "ps aux");
                break;
            case 8:
                execute_command(client_socket, "pstree");
                break;
            case 9:
                close(client_socket);
                close(server_fd);
                exit(EXIT_SUCCESS);
                break;
            default:
                send(client_socket, "Comando non valido\n", 19, 0);
                break;
        }
    }

    return 0;
}
