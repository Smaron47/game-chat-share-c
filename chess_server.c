// chess_server.c

#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

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





#define PORT 12345
#define BUFFER_SIZE 1024

void initializeServer(char board[8][8], SOCKET player1, SOCKET player2);
void updateBoard(char board[8][8], char move[5]);
void sendBoardToClients(char board[8][8], SOCKET player1, SOCKET player2);


int main() {
    printf("Chess Server is running\n");

    // Add the server logic here
    WSADATA wsa;
    SOCKET serverSocket, player1Socket, player2Socket;
    struct sockaddr_in server, client;
    int c;
    char board[8][8];

    // Initialize Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        printf("Failed. Error Code : %d", WSAGetLastError());
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
    listen(serverSocket, 2);
    printf("Waiting for players to connect...\n");

    // Accept player 1
    c = sizeof(struct sockaddr_in);
    if ((player1Socket = accept(serverSocket, (struct sockaddr*)&client, &c)) == INVALID_SOCKET) {
        printf("Accept failed with error code : %d", WSAGetLastError());
        return 1;
    }

    printf("Player 1 connected.\n");

    // Accept player 2
    if ((player2Socket = accept(serverSocket, (struct sockaddr*)&client, &c)) == INVALID_SOCKET) {
        printf("Accept failed with error code : %d", WSAGetLastError());
        return 1;
    }

    printf("Player 2 connected.\n");

    // Initialize the chess board
    initializeBoard(board);
    sendBoardToClients(board, player1Socket, player2Socket);

    // Game loop
    while (1) {
        char move[BUFFER_SIZE];

        // Receive move from player 1
        recv(player1Socket, move, BUFFER_SIZE, 0);
        updateBoard(board, move);
        sendBoardToClients(board, player1Socket, player2Socket);

        // Receive move from player 2
        recv(player2Socket, move, BUFFER_SIZE, 0);
        updateBoard(board, move);
        sendBoardToClients(board, player1Socket, player2Socket);
    }

    closesocket(player1Socket);
    closesocket(player2Socket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}

void initializeServer(char board[8][8], SOCKET player1, SOCKET player2) {
    initializeBoard(board);
    sendBoardToClients(board, player1, player2);
}

void updateBoard(char board[8][8], char move[5]) {
    // Implement the logic to update the chess board based on the move
    // For simplicity, assume that the move is a string like "e2e4" (from e2 to e4)
    // This function does not perform move validation
    int fromX = move[0] - 'a';
    int fromY = '8' - move[1];
    int toX = move[2] - 'a';
    int toY = '8' - move[3];

    board[toY][toX] = board[fromY][fromX];
    board[fromY][fromX] = '.';
}

void sendBoardToClients(char board[8][8], SOCKET player1, SOCKET player2) {
    char buffer[BUFFER_SIZE];
    // Prepare the board for sending
    sprintf(buffer, "board\n");
    for (int i = 0; i < 8; ++i) {
        for (int j = 0; j < 8; ++j) {
            sprintf(buffer + strlen(buffer), "%c", board[i][j]);
        }
        sprintf(buffer + strlen(buffer), "\n");
    }

    // Send the board to both players
    send(player1, buffer, strlen(buffer), 0);
    send(player2, buffer, strlen(buffer), 0);
}
