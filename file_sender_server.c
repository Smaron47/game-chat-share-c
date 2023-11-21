// file_sender_server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define PORT 12345
#define BUFFER_SIZE 1024




void getFileName(const char *fullPath, char *fileName) {
    // Replace single backslashes with double backslashes
    char sanitizedPath[256];  // Adjust the size as needed
    int j = 0;
    for (int i = 0; i < strlen(fullPath); ++i) {
        if (fullPath[i] == '\\') {
            sanitizedPath[j++] = '\\';  // Add an additional backslash
        }
        sanitizedPath[j++] = fullPath[i];
    }
    sanitizedPath[j] = '\0';

    // Find the last occurrence of the path separator (either '/' or '\')
    const char *lastSlash = strrchr(sanitizedPath, '/');
    const char *lastBackslash = strrchr(sanitizedPath, '\\');

    // Choose the last occurrence of the path separator
    const char *lastSeparator = (lastSlash > lastBackslash) ? lastSlash : lastBackslash;

    // If a separator is found, copy the file name after the separator
    if (lastSeparator != NULL) {
        strcpy(fileName, lastSeparator + 1);
    } else {
        // If no separator is found, the full path is the file name
        strcpy(fileName, sanitizedPath);
    }
}



void receiveFile(SOCKET clientSocket) {
    char fileName[BUFFER_SIZE];
    char name[BUFFER_SIZE];
    int fileNameSize;
    FILE* file;

    // Receive the file name size
    recv(clientSocket, (char*)&fileNameSize, sizeof(int), 0);

    // Receive the file name
    recv(clientSocket, fileName, fileNameSize, 0);
    getFileName(fileName, name);
    // Open the file for writing
    file = fopen(name, "wb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Receive and write file data
    char buffer[BUFFER_SIZE];
    int bytesRead;
    while ((bytesRead = recv(clientSocket, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytesRead, file);
    }

    fclose(file);
    printf("File received successfully: %s\n", fileName);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        return 1;
    }

    // Use argv[1] as the port number
    printf("File Sender Server is running on port %s\n", argv[1]);

    WSADATA wsa;
    SOCKET serverSocket, newClient;
    struct sockaddr_in server, client;
    int c;

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
    listen(serverSocket, SOMAXCONN);

    printf("Server listening on port %d...\n", PORT);

    // Accept and handle incoming connections
    while (1) {
        c = sizeof(struct sockaddr_in);
        newClient = accept(serverSocket, (struct sockaddr*)&client, &c);
        if (newClient == INVALID_SOCKET) {
            printf("Accept failed with error code : %d", WSAGetLastError());
            return 1;
        }

        // Handle the client in a separate function (asynchronously)
        // You can use threading or asynchronous I/O for concurrent handling of multiple clients
        receiveFile(newClient);

        closesocket(newClient);
    }

    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
