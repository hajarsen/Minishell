# Minishell
groupe hsennane and sel-jari Minishell.


### 🔹 **Input Handling (Readline Library)**

These are functions from the `readline` library, used for interactive command-line input:

* `readline`: Reads a line of input from the user with editing features.
* `add_history`: Adds the entered line to the history list.
* `rl_clear_history`: Clears the history list.
* `rl_on_new_line`: Prepares for a new line of input.
* `rl_replace_line`: Replaces the current line in the input buffer.
* `rl_redisplay`: Redisplays the current line buffer (after a change).

---

### 🔹 **Output / Printing**

* `printf`: Formatted output to stdout.
* `write`: Low-level function to write bytes to a file descriptor.
* `perror`: Prints a descriptive error message based on `errno`.
* `strerror`: Returns the string description of an error number.
* `tputs`: Outputs a string with padding for terminal control.
* `tgoto`: Computes cursor movement control string.
* `tgetstr`, `tgetnum`, `tgetflag`: Get terminal capability strings, numbers, and flags.
* `tgetent`: Loads terminal capabilities from the termcap database.

---

### 🔹 **Memory Management**

* `malloc`: Allocates memory dynamically.
* `free`: Frees dynamically allocated memory.

---

### 🔹 **File and Directory Operations**

* `open`: Opens a file and returns a file descriptor.
* `read`: Reads data from a file descriptor.
* `close`: Closes a file descriptor.
* `access`: Checks file accessibility (e.g., readable, writable).
* `unlink`: Deletes a file.
* `stat`, `lstat`, `fstat`: Get file status info (different levels of symbolic link handling).
* `opendir`: Opens a directory for reading.
* `readdir`: Reads the next entry in the directory.
* `closedir`: Closes an open directory stream.

---

### 🔹 **Process Management**

* `fork`: Creates a new process by duplicating the current process.
* `wait`, `waitpid`, `wait3`, `wait4`: Wait for state changes in child processes.
* `execve`: Replaces the current process with a new executable.
* `exit`: Terminates the current process.

---

### 🔹 **Signal Handling**

* `signal`: Installs a signal handler.
* `sigaction`: More advanced signal handling setup.
* `sigemptyset`: Initializes a signal set to empty.
* `sigaddset`: Adds a signal to a signal set.
* `kill`: Sends a signal to a process.

---

### 🔹 **Terminal and TTY**

* `isatty`: Checks if a file descriptor is associated with a terminal.
* `ttyname`: Gets the name of the terminal associated with a file descriptor.
* `ttyslot`: Returns the index of the terminal slot.
* `ioctl`: Performs device-specific input/output operations.
* `tcgetattr`, `tcsetattr`: Get or set terminal attributes.

---

### 🔹 **Environment and Path**

* `getenv`: Retrieves environment variable value.
* `getcwd`: Gets the current working directory.
* `chdir`: Changes the current working directory.

---

### 🔹 **File Descriptor Operations**

* `dup`: Duplicates a file descriptor.
* `dup2`: Duplicates a file descriptor to a given number.
* `pipe`: Creates a pair of connected file descriptors (pipe).
