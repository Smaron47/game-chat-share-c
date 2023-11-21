// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <windows.h>

#define PORT 12345
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 10

SOCKET clients[MAX_CLIENTS];
int numClients = 0;
CRITICAL_SECTION cs;

DWORD WINAPI handleClient(LPVOID lpParam) {
    SOCKET newClient = *((SOCKET*)lpParam);
    char buffer[BUFFER_SIZE];

    while (1) {
        int bytesRead = recv(newClient, buffer, BUFFER_SIZE, 0);
        if (bytesRead <= 0) {
            break; // Client disconnected
        }

        // Broadcast the message to all clients
        EnterCriticalSection(&cs);
        for (int i = 0; i < numClients; ++i) {
            if (clients[i] != newClient) {
                send(clients[i], buffer, bytesRead, 0);
            }
        }
        LeaveCriticalSection(&cs);
    }

    // Remove the client from the list
    EnterCriticalSection(&cs);
    for (int i = 0; i < numClients; ++i) {
        if (clients[i] == newClient) {
            for (int j = i; j < numClients - 1; ++j) {
                clients[j] = clients[j + 1];
            }
            numClients--;
            break;
        }
    }
    LeaveCriticalSection(&cs);

    closesocket(newClient);
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Use argv[1] as the port number
    printf("Chat Server is running on port %s\n", argv[1]);    WSADATA wsa;
    SOCKET serverSocket, newClient;
    struct sockaddr_in server, client;
    int c, bytesRead;
    char buffer[BUFFER_SIZE];
    HANDLE thread;

    InitializeCriticalSection(&cs);

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return 1;
    }

    // Create a socket
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return 1;
    }

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        printf("Bind failed with error code : %d", WSAGetLastError());
        return 1;
    }

    // Listen to incoming connections
    listen(serverSocket, MAX_CLIENTS);

    printf("Server listening on port %d...\n", PORT);

    // Accept and handle incoming connections
    while (1) {
        c = sizeof(struct sockaddr_in);
        newClient = accept(serverSocket, (struct sockaddr*)&client, &c);
        if (newClient == INVALID_SOCKET) {
            printf("Accept failed with error code : %d", WSAGetLastError());
            return 1;
        }

        // Add the new client to the list
        EnterCriticalSection(&cs);
        clients[numClients++] = newClient;
        LeaveCriticalSection(&cs);

        // Create a new thread to handle the client
        thread = CreateThread(NULL, 0, handleClient, (LPVOID)&newClient, 0, NULL);
        if (thread == NULL) {
            perror("Could not create thread");
            return 1;
        }
    }

    closesocket(serverSocket);
    WSACleanup();

    DeleteCriticalSection(&cs);

    return 0;
}
