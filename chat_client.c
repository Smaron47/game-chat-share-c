// client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

SOCKET clientSocket;
CRITICAL_SECTION cs;

DWORD WINAPI receiveMessages(LPVOID lpParam) {
    char buffer[BUFFER_SIZE];
    int bytesRead;

    while (1) {
        bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            printf("Server disconnected.\n");
            break;
        }

        buffer[bytesRead] = '\0';
        printf("%s", buffer);
    }

    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s <server_ip> <port>\n", argv[0]);
        return 1;
    }

    // Use argv[1] as the server IP and argv[2] as the port number
    printf("Chat Client is connecting to server %s on port %s\n", argv[1], argv[2]);

    WSADATA wsa;
    struct sockaddr_in server;
    HANDLE thread;
    char message[BUFFER_SIZE];

    InitializeCriticalSection(&cs);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr(SERVER_IP);
    server.sin_family = AF_INET;
    server.sin_port = htons(PORT);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    // Create a new thread to receive messages
    thread = CreateThread(NULL, 0, receiveMessages, NULL, 0, NULL);
    if (thread == NULL) {
        perror("Could not create thread");
        return 1;
    }

    // Main loop to send messages
    while (1) {
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);

        // Send the message to the server
        send(clientSocket, message, strlen(message), 0);
    }

    closesocket(clientSocket);
    WSACleanup();

    DeleteCriticalSection(&cs);

    return 0;
}
