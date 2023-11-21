#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>

#define MAX_NOTES 100
#define MAX_NOTE_LENGTH 256
#define FILENAME "notes.txt"

// Function to play a sound notification
void playNotificationSound() {
    Beep(1000, 500); // Beep at 1000 Hz for 500 milliseconds
}

// Function to add a new note
void addNote() {
    FILE* file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char note[MAX_NOTE_LENGTH];
    printf("Enter your note: ");
    fgets(note, sizeof(note), stdin);

    // Remove newline character from the end of the note
    note[strcspn(note, "\n")] = '\0';

    fprintf(file, "%s\n", note);
    fclose(file);

    printf("Note added successfully!\n");
}

// Function to view all notes
void viewNotes() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char note[MAX_NOTE_LENGTH];
    int noteCount = 0;

    printf("Your notes:\n");

    while (fgets(note, sizeof(note), file) != NULL) {
        printf("%d. %s", ++noteCount, note);
    }

    fclose(file);

    if (noteCount == 0) {
        printf("No notes found.\n");
    }
}

// Function to delete a note
void deleteNote() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    char note[MAX_NOTE_LENGTH];
    char notes[MAX_NOTES][MAX_NOTE_LENGTH];
    int noteCount = 0;

    // Read notes into an array
    while (fgets(note, sizeof(note), file) != NULL && noteCount < MAX_NOTES) {
        strcpy(notes[noteCount], note);
        noteCount++;
    }

    fclose(file);

    // Display notes for selection
    printf("Select a note to delete:\n");
    for (int i = 0; i < noteCount; ++i) {
        printf("%d. %s", i + 1, notes[i]);
    }

    if (noteCount == 0) {
        printf("No notes found.\n");
        return;
    }

    // Get user input for note to delete
    int selection;
    printf("Enter the number of the note to delete: ");
    scanf("%d", &selection);

    if (selection < 1 || selection > noteCount) {
        printf("Invalid selection.\n");
        return;
    }

    // Remove the selected note
    for (int i = selection - 1; i < noteCount - 1; ++i) {
        strcpy(notes[i], notes[i + 1]);
    }

    // Write remaining notes back to the file
    file = fopen(FILENAME, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    for (int i = 0; i < noteCount - 1; ++i) {
        fprintf(file, "%s", notes[i]);
    }

    fclose(file);

    printf("Note deleted successfully!\n");
}

int main() {
    printf("Reminder and Note System\n");

    // Check for previous notes
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
    } else {
        fclose(file);
        viewNotes(); // Display existing notes when the program starts
    }

    while (1) {
        printf("\nOptions:\n");
        printf("1. Add Note\n");
        printf("2. View Notes\n");
        printf("3. Delete Note\n");
        printf("4. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addNote();
                playNotificationSound();
                break;
            case 2:
                viewNotes();
                break;
            case 3:
                deleteNote();
                break;
            case 4:
                printf("Exiting program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
