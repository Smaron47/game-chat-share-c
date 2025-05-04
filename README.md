**Multi-Tool Console Application – C Program Documentation**

---

## Table of Contents

1. Introduction & Overview
2. Compilation & Execution
3. Directory & Dependencies
4. User Interface & Menu Flow
5. Function Summaries

   * 5.1 `sendFile()`
   * 5.2 `receiveFile()`
   * 5.3 `chat()`
   * 5.4 `fileConvert()`
   * 5.5 `reminderSystem()`
   * 5.6 `notesSystem()`
   * 5.7 `chessGame()`
6. Detailed Function Descriptions

   * 6.1 File Transfer Tools
   * 6.2 Chat Tool
   * 6.3 File Converter Tool
   * 6.4 Reminder System Tool
   * 6.5 Notes System Tool
   * 6.6 Chess Game Tool
7. System Commands & External Executables
8. Error Handling & Edge Cases
9. Extending & Customization
10. Best Practices & Security Considerations
11. Example Usage Scenarios
12. Future Enhancements
13. Keywords & Index
14. License & Author

---

## 1. Introduction & Overview

The **Multi-Tool Console Application** is a non-graphical, menu-driven program written in C. It serves as a unified launcher for various standalone utilities:

* **File Transfer**: Send or receive files via client/server executables
* **Chat**: Real-time messaging between server and client
* **File Conversion**: Invoke an external file converter
* **Reminder System**: Launch a reminder scheduler
* **Notes System**: Quick note-taking utility
* **Chess Game**: Two-player chess via networked client/server

Each feature is implemented as a separate external executable, and the main program uses `system()` calls to spawn new console windows and run the corresponding tool.

---

## 2. Compilation & Execution

**Prerequisites:**

* C compiler (e.g., `gcc` or MSVC)
* Windows environment for `start cmd /k` commands (adjust for Linux/macOS)
* Executables: `file_server.exe`, `file_client.exe`, `chat_server.exe`, `chat_client.exe`, `file_converter.exe`, `reminder.exe`, `add_note.exe`, `chess_server.exe`, `chess_client.exe` available in working directory or PATH

**Compile:**

```bash
gcc -o multitool app.c
```

**Run:**

```bash
multitool.exe
```

---

## 3. Directory & Dependencies

```
project_root/
├── app.c               # Multi-tool launcher source code
├── file_server.exe     # File transfer server
├── file_client.exe     # File transfer client
├── chat_server.exe     # Chat server
├── chat_client.exe     # Chat client
├── file_converter.exe  # File conversion utility
├── reminder.exe        # Reminder scheduler
├── add_note.exe        # Notes utility
├── chess_server.exe    # Chess game server
├── chess_client.exe    # Chess game client
└── multitool.exe       # Compiled launcher program
```

---

## 4. User Interface & Menu Flow

Upon starting `multitool.exe`, the console displays:

```
Welcome to the Multi-Tool Program
1. Send File
2. Receive File
3. Chat
4. File Convert
5. Reminder System
6. Notes System
7. Chess Game
0. Exit
Enter your choice (0-7):
```

The user inputs a number to select a tool. After the chosen tool runs in its own console window, the main menu reappears until the user selects `0` to exit.

---

## 5. Function Summaries

### 5.1 `sendFile()`

Prompts for file path and server IP, then launches the file client executable to send the file.

### 5.2 `receiveFile()`

Starts the file server executable to receive incoming files.

### 5.3 `chat()`

Asks whether to run as server or client and launches chat executables accordingly.

### 5.4 `fileConvert()`

Invokes the file converter executable in a new console window.

### 5.5 `reminderSystem()`

Launches the reminder scheduler executable.

### 5.6 `notesSystem()`

Launches the notes utility executable for quick note-taking.

### 5.7 `chessGame()`

Prompts for server/client role and starts the chess game executables.

---

## 6. Detailed Function Descriptions

### 6.1 File Transfer Tools

#### `sendFile()`

* **Inputs**: File path and server IP via `scanf`.
* **Behavior**: Constructs a `start cmd /k file_client.exe <filePath> <serverIP> 127.0.0.1 12346` command string and calls `system()` to run it in a new window.
* **External Dependence**: Requires `file_client.exe` and the server to be running.

#### `receiveFile()`

* **Behavior**: Calls `system("start cmd /k file_server.exe 12346")` to listen on port 12346.
* **Output**: Console displays "Receiving File..."

### 6.2 Chat Tool

#### `chat()`

* **Inputs**: Server/client selection and optionally server IP.
* **Behavior**:

  * If server: launches `chat_server.exe 12345` and `chat_client.exe 127.0.0.1 12345`.
  * If client: prompts for IP and runs `chat_client.exe <serverIP> 12345`.

### 6.3 File Converter Tool

#### `fileConvert()`

* **Behavior**: Runs `system("start cmd /k file_converter.exe")` and prints "Converting File...".

### 6.4 Reminder System Tool

#### `reminderSystem()`

* **Behavior**: Runs `system("start cmd /k reminder.exe")` and prints "Reminder System...".

### 6.5 Notes System Tool

#### `notesSystem()`

* **Behavior**: Runs `system("start cmd /k add_note.exe")` and prints "Notes System...".

### 6.6 Chess Game Tool

#### `chessGame()`

* **Inputs**: Server/client selection.
* **Behavior**:

  * Server: launches `chess_server.exe` then `chess_client.exe 127.0.0.1`.
  * Client: launches `chess_client.exe 127.0.0.1`.

---

## 7. System Commands & External Executables

| Tool            | Command Template                                                        |
| --------------- | ----------------------------------------------------------------------- |
| Send File       | `file_server.exe` / `file_client.exe <file> <serverIP> 127.0.0.1 12346` |
| Receive File    | `file_server.exe 12346`                                                 |
| Chat            | `chat_server.exe 12345`, `chat_client.exe <IP> 12345`                   |
| File Converter  | `file_converter.exe`                                                    |
| Reminder System | `reminder.exe`                                                          |
| Notes System    | `add_note.exe`                                                          |
| Chess Game      | `chess_server.exe`, `chess_client.exe <IP>`                             |

---

## 8. Error Handling & Edge Cases

* **Invalid Choice**: Main menu prints "Invalid choice..." and reprompts.
* **Missing Executables**: `system()` will fail; recommend verifying PATH.
* **Input Errors**: Non-numeric menu inputs lead to undefined behavior; enhance with `fgets` + `strtol`.

---

## 9. Extending & Customization

* **Cross-Platform**: Replace `start cmd /k` with appropriate Linux/macOS commands (e.g., `xterm -e`).
* **Argument Validation**: Add checks for file existence and valid IP format.
* **Dynamic Ports**: Allow custom port input instead of hardcoded defaults.
* **Logging**: Capture `system()` return codes for diagnostics.

---

## 10. Best Practices & Security Considerations

* **Avoid `system()`**: Use `exec` family or platform APIs to mitigate shell injection.
* **Input Sanitization**: Validate all user inputs (file paths, IPs, port numbers).
* **Credentials**: For chat or file transfers, implement authentication if needed.
* **Resource Cleanup**: Ensure server processes are terminated gracefully.

---

## 11. Example Usage Scenarios

1. **File Sharing**: User A runs `receiveFile()`, User B runs `sendFile()` pointing to User A’s IP.
2. **Group Chat**: One user chooses server role; others choose client and connect.
3. **Chess Match**: Two computers on a LAN run chess in server/client mode.

---

## 12. Future Enhancements

* Integrate all tools into a single network protocol rather than separate executables.
* Provide a graphical UI wrapper using ncurses or a GUI toolkit.
* Support encryption for file and chat communications.
* Implement user authentication and secure channels (TLS).

---

## 13. Keywords & Index

```
C console multitool
file transfer C system calls
chat server client C
reminder scheduler C
notes utility C
chess game network C
system exec documentation
menu-driven C application
C console multitool
file transfer C system calls
chat server client C
reminder scheduler C
notes utility C
chess game network C
system exec documentation
menu-driven C application
```

---

## 14. License & Author

**Author:** Smaron Biswas
**Year:** 2024
**License:** MIT License

This documentation and code are released under the MIT License.
