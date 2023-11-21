
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <time.h>

#define MAX_REMINDERS 100
#define MAX_NOTE_LENGTH 256
#define FILENAME "reminders.txt"

// Structure to store reminder information
typedef struct {
    int hour;
    int minute;
    char note[MAX_NOTE_LENGTH];
} Reminder;

// Function to play a sound notification
void playNotificationSound() {
    Beep(1000, 500); // Beep at 1000 Hz for 500 milliseconds
}

// Function to add a new reminder
void addReminder() {
    FILE* file = fopen(FILENAME, "a");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Reminder newReminder;

    printf("Enter reminder time (HH MM): ");
    scanf("%d %d", &newReminder.hour, &newReminder.minute);

    printf("Enter reminder note: ");
    fflush(stdin); // Clear input buffer
    fgets(newReminder.note, sizeof(newReminder.note), stdin);

    fprintf(file, "%02d %02d %s", newReminder.hour, newReminder.minute, newReminder.note);
    fclose(file);

    printf("Reminder added successfully!\n");
}

// Function to check and notify reminders
void checkReminders() {
    FILE* file = fopen(FILENAME, "r");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    Reminder reminders[MAX_REMINDERS];
    int reminderCount = 0;

    while (fscanf(file, "%d %d %[^\n]", &reminders[reminderCount].hour,
                  &reminders[reminderCount].minute, reminders[reminderCount].note) != EOF &&
           reminderCount < MAX_REMINDERS) {
        reminderCount++;
    }

    fclose(file);

    time_t currentTime = time(NULL);
    struct tm* localTime = localtime(&currentTime);

    for (int i = 0; i < reminderCount; ++i) {
        if (reminders[i].hour == localTime->tm_hour && reminders[i].minute == localTime->tm_min) {
            playNotificationSound();
            printf("Reminder: %s\n", reminders[i].note);
        }
    }
}

// Function to run in a separate thread for periodic reminders check
DWORD WINAPI ReminderThread(LPVOID lpParam) {
    while (1) {
        // Check reminders every minute
        checkReminders();
        Sleep(60000); // Sleep for 1 minute
    }
    return 0;
}

int main() {
    printf("Reminder System\n");

    // Create a thread for periodic reminders check
    HANDLE hThread = CreateThread(NULL, 0, ReminderThread, NULL, 0, NULL);
    if (hThread == NULL) {
        perror("Error creating thread");
        return 1;
    }

    while (1) {
        printf("\nOptions:\n");
        printf("1. Add Reminder\n");
        printf("2. Exit\n");

        int choice;
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addReminder();
                break;
            case 2:
                printf("Exiting program. Goodbye!\n");

                // Wait for the thread to finish before exiting
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);

                return 0;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
