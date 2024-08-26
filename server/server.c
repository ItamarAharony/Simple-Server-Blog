#include "chat.h"
#include "queue.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345
#define MAX_CLIENTS 100

int client_sockets[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void broadcast_message(char *message, int sender_socket);
void *handle_client(void *client_socket);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

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

    printf("Server listening on port %d\n", PORT);

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        pthread_mutex_lock(&clients_mutex);
        client_sockets[client_count++] = new_socket;
        pthread_mutex_unlock(&clients_mutex);

        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, (void *)&new_socket) != 0) {
            perror("pthread_create");
        }
    }

    return 0;
}

void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] != sender_socket) {
            send(client_sockets[i], message, strlen(message), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;
    char buffer[1024];
    int valread;

    while ((valread = read(sock, buffer, 1024)) > 0) {
        buffer[valread] = '\0';

        // Format the message with client socket number
        char message[1100];
        snprintf(message, sizeof(message), "client %d: \"%s\"", sock, buffer);

        // Print to server terminal
        printf("%s\n", message);

        // Broadcast the message to all clients
        broadcast_message(message, sock);
    }

    close(sock);

    // Remove the client from the list
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = client_sockets[--client_count];
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

