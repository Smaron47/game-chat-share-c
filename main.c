#include <stdio.h>
#include <stdlib.h>

void sendFile() {
    char filePath[100];
    char serverIP[20];
    char command[100];

    printf("Enter the file path: ");
    scanf("%s", filePath);

    printf("Enter the server IP: ");
    scanf("%s", serverIP);

    // system("start cmd /k file_server.exe 12346");  // Use appropriate port number
    sprintf(command,"start cmd /k file_client.exe %s %s 127.0.0.1 12346", filePath, serverIP);  // Use server IP and port
    system(command);
}

void receiveFile() {
    system("start cmd /k file_server.exe 12346");
    printf("Receiving File...\n");
}

void chat() {
    int isServer;
    char serverIP[20];
    char command[100];

    printf("Are you the server or client? (1 for Server / 0 for Client): ");
    scanf("%d", &isServer);

    if (isServer) {
        system("start cmd /k chat_server.exe 12345");  // Use appropriate port number
        system("start cmd /k chat_client.exe 127.0.0.1 12345");
    } else {
        printf("Enter the server IP: ");
        scanf("%s", serverIP);
        sprintf(command,"start cmd /k chat_client.exe %s 12345", serverIP);  // Use server IP and port
        system(command);
    }
}

void fileConvert() {


    system("start cmd /k file_converter.exe");
    printf("Converting File...\n");
}

void reminderSystem() {
    system("start cmd /k reminder.exe");
    printf("Reminder System...\n");
}

void notesSystem() {
    system("start cmd /k add_note.exe");
    printf("Notes System...\n");
}

void chessGame() {
    int isServer;

    printf("Do you want to be the server or client for the chess game?\n");
    printf("Enter 1 for Server / 0 for Client: ");
    scanf("%d", &isServer);

    if (isServer) {
        system("start cmd /k chess_server.exe");  // Use appropriate port number
        system("start cmd /k chess_client.exe 127.0.0.1");
    } else {
        system("start cmd /k chess_client.exe 127.0.0.1");  // Use server IP and port
    }
}

void displayMenu() {
    printf("Welcome to the Multi-Tool Program\n");
    printf("1. Send File\n");
    printf("2. Receive File\n");
    printf("3. Chat\n");
    printf("4. File Convert\n");
    printf("5. Reminder System\n");
    printf("6. Notes System\n");
    printf("7. Chess Game\n");
    printf("0. Exit\n");
}

int main() {
    int choice;

    do {
        displayMenu();
        printf("Enter your choice (0-7): ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                sendFile();
                break;

            case 2:
                receiveFile();
                break;

            case 3:
                chat();
                break;

            case 4:
                fileConvert();
                break;

            case 5:
                reminderSystem();
                break;

            case 6:
                notesSystem();
                break;

            case 7:
                chessGame();
                break;

            case 0:
                printf("Exiting program. Goodbye!\n");
                break;

            default:
                printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);

    return 0;
}
