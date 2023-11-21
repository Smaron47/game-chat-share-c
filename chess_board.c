// chess_board.c

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



int main() {
    char chessBoard[8][8];
    initializeBoard(chessBoard);
    drawBoard(chessBoard);

    return 0;
}