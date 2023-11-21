// file_sender_client.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

// Function to send a file to a specific server
void sendFile(const char* filePath, const char* serverIP, int port) {
    WSADATA wsa;
    struct sockaddr_in server;
    SOCKET clientSocket;

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code: %d", WSAGetLastError());
        return;
    }

    // Create a socket
    if ((clientSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        printf("Could not create socket : %d", WSAGetLastError());
        return;
    }

    // Prepare the sockaddr_in structure
    server.sin_addr.s_addr = inet_addr(serverIP);
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return;
    }

    // Send the file to the server
    FILE* file = fopen(filePath, "rb");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Send the file name size
    int fileNameSize = strlen(filePath) + 1; // Include the null terminator
    send(clientSocket, (char*)&fileNameSize, sizeof(int), 0);

    // Send the file name
    send(clientSocket, filePath, fileNameSize, 0);

    // Send file data
    char buffer[BUFFER_SIZE];
    int bytesRead;
    while ((bytesRead = fread(buffer, 1, sizeof(buffer), file)) > 0) {
        send(clientSocket, buffer, bytesRead, 0);
    }

    fclose(file);
    printf("File sent successfully: %s\n", filePath);

    // Close the client socket
    closesocket(clientSocket);
    WSACleanup();
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Usage: %s <file_path> <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    // Use argv[1] as the file path, argv[2] as the server IP, and argv[3] as the server port
    printf("File Sender Client is sending file %s to server %s on port %s\n", argv[1], argv[2], argv[3]);

    // List of server addresses and ports
    const char* serverAddresses[] = { "127.0.0.1" };
    const int serverPorts[] = { 12345 };

    // Loop through the list of servers and send files
    for (int i = 0; i < sizeof(serverAddresses) / sizeof(serverAddresses[0]); ++i) {
        sendFile("C:\\Users\\eioBuy Tech\\Desktop\\experiment.txt", serverAddresses[i], serverPorts[i]);
    }

    return 0;
}
