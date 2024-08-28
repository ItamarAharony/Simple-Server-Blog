#include "chat.h"
#include "queue.h"
#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 12345         // The port number on which the server will listen
#define MAX_CLIENTS 100    // Maximum number of clients that can be connected at the same time

int client_sockets[MAX_CLIENTS];           // Array to hold client socket descriptors
int client_count = 0;                      // Count of currently connected clients
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;  // Mutex to protect access to the client_sockets array

// Function to broadcast a message to all clients except the sender
void broadcast_message(char *message, int sender_socket);

// Function to handle communication with a single client
void *handle_client(void *client_socket);

int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    // Create a socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Set up the server address struct
    address.sin_family = AF_INET;              // IPv4
    address.sin_addr.s_addr = INADDR_ANY;      // Accept connections from any IP address
    address.sin_port = htons(PORT);            // Port number, converted to network byte order

    // Bind the socket to the specified port and IP
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Start listening for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    while (1) {
        // Accept a new client connection
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }

        // Add the new client to the client_sockets array
        pthread_mutex_lock(&clients_mutex);
        client_sockets[client_count++] = new_socket;
        pthread_mutex_unlock(&clients_mutex);

        // Create a new thread to handle the new client
        pthread_t tid;
        if (pthread_create(&tid, NULL, handle_client, (void *)&new_socket) != 0) {
            perror("pthread_create");
        }
    }

    return 0;
}

// Broadcasts a message to all connected clients except the sender
void broadcast_message(char *message, int sender_socket) {
    pthread_mutex_lock(&clients_mutex);  // Lock the client_sockets array
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] != sender_socket) {
            send(client_sockets[i], message, strlen(message), 0);  // Send the message to each client
        }
    }
    pthread_mutex_unlock(&clients_mutex);  // Unlock the client_sockets array
}

// Handles communication with a single client
void *handle_client(void *client_socket) {
    int sock = *(int *)client_socket;  // Get the client socket descriptor
    char buffer[1024];                 // Buffer to store messages from the client
    int valread;

    while ((valread = read(sock, buffer, 1024)) > 0) {
        buffer[valread] = '\0';  // Null-terminate the received message

        // Format the message to include the client's socket number
        char message[1100];
        snprintf(message, sizeof(message), "client %d: %s", sock, buffer);

        // Print the message to the server's terminal
        printf("%s\n", message);

        // Broadcast the message to all other clients
        broadcast_message(message, sock);
    }

    close(sock);  // Close the client socket

    // Remove the client from the client_sockets array
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = client_sockets[--client_count];  // Replace the removed client with the last client in the array
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);

    return NULL;
}

