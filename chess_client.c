// chess_client.c

#include <stdio.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

#include <stdio.h>
#include <windows.h>



void drawBoard(char board[8][8]) {
    printf("  a b c d e f g h\n");
    for (int i = 0; i < 8; ++i) {
        printf("%d ", 8 - i);
        for (int j = 0; j < 8; ++j) {
            printf("%c ", board[i][j]);
        }
        printf("\n");
    }
}

void initializeBoard(char board[8][8]) {
    // Implement the logic to initialize the chess board with pieces
    // For simplicity, let's use placeholders
    // '.' represents an empty square
    // 'P', 'R', 'N', 'B', 'Q', 'K' represent white pieces
    // 'p', 'r', 'n', 'b', 'q', 'k' represent black pieces
    char initialBoard[8][8] = {
        {'r', 'n', 'b', 'q', 'k', 'b', 'n', 'r'},
        {'p', 'p', 'p', 'p', 'p', 'p', 'p', 'p'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'.', '.', '.', '.', '.', '.', '.', '.'},
        {'P', 'P', 'P', 'P', 'P', 'P', 'P', 'P'},
        {'R', 'N', 'B', 'Q', 'K', 'B', 'N', 'R'}
    };

    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            board[i][j] = initialBoard[i][j];
        }
    }
}




void displayBoard(char buffer[BUFFER_SIZE]);

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <server_ip>\n", argv[0]);
        return 1;
    }

    // Use argv[1] as the server IP
    printf("Chess Client is connecting to server %s\n", argv[1]);
    WSADATA wsa;
    SOCKET clientSocket;
    struct sockaddr_in server;
    char buffer[BUFFER_SIZE];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
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

    // Connect to the server
    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    printf("Connected to the server.\n");

    // Receive and display the initial chess board
    recv(clientSocket, buffer, BUFFER_SIZE, 0);
    displayBoard(buffer);

    // Game loop
    while (1) {
        char move[BUFFER_SIZE];

        // Get the move from the user
        printf("Enter your move (e.g., e2e4): ");
        fgets(move, BUFFER_SIZE, stdin);

        // Send the move to the server
        send(clientSocket, move, strlen(move), 0);

        // Receive and display the updated chess board
        recv(clientSocket, buffer, BUFFER_SIZE, 0);
        displayBoard(buffer);
    }

    closesocket(clientSocket);
    WSACleanup();

    return 0;
}

void displayBoard(char buffer[BUFFER_SIZE]) {
    // Display the chess board received from the server
    printf("%s", buffer);
}
