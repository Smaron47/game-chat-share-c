#include <stdio.h>
#include <stdlib.h>
#include<string.h>

#define MAX_PATH_LENGTH 256
#define MAX_EXT_LENGTH 10

int main() {
    char filePath[MAX_PATH_LENGTH];
    char newExtension[MAX_EXT_LENGTH];

    // Get file path from the user
    printf("Enter the file path: ");
    fgets(filePath, sizeof(filePath), stdin);

    // Remove trailing newline character from the file path
    strtok(filePath, "\n");

    // Get the new extension from the user
    printf("Enter the new extension (without the dot): ");
    fgets(newExtension, sizeof(newExtension), stdin);

    // Remove trailing newline character from the extension
    strtok(newExtension, "\n");

    // Build the shell command to change the file extension
    char command[MAX_PATH_LENGTH + MAX_EXT_LENGTH + 20];
    snprintf(command, sizeof(command), "ren \"%s\" *.%s", filePath, newExtension);

    // Execute the shell command
    int result = system(command);

    if (result == 0) {
        printf("File extension changed successfully.\n");
    } else {
        printf("Error changing file extension.\n");
    }

    return 0;
}
