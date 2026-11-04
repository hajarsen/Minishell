*This project was developed as part of the 42 curriculum by hsennane and sel-jari.*

# Minishell

## Description

**Minishell** is a small Unix-like command interpreter written in **C**. The project explores how a shell accepts a command line, interprets quoting and operators, expands environment variables, and runs commands using Unix system calls.

The implementation is organized into input parsing, environment management, command execution, redirections, pipelines, built-in commands, and signal handling.

## Instructions

### Requirements

A Unix/Linux development environment with a C compiler, `make`, and the **GNU Readline** development library. The project also includes its own `libft`.

On Ubuntu, the Readline development package is commonly installed with:

```bash
sudo apt install libreadline-dev
```

### Compilation

From the repository root:

```bash
make
./minishell
```

Makefile targets:

| Command | Action |
|---|---|
| `make` | Build the `minishell` executable |
| `make clean` | Remove object files |
| `make fclean` | Remove object files and the executable |
| `make re` | Rebuild the project |

### Example Commands

Try these commands **inside Minishell**:

```bash
pwd
echo "Hello, Minishell!"
echo -n "no trailing newline"
cd ..
pwd
export MY_VAR=42
echo "$MY_VAR"
env
ls -l | wc -l
echo "test" > output.txt
cat < output.txt
echo "another line" >> output.txt
cat << EOF
hello
EOF
```

Use `exit` to leave the shell. The examples demonstrate intended shell features; they are not a substitute for running tests on your system.

## Features

| Area | Implementation |
|---|---|
| Interactive prompt | Reads user commands with GNU Readline |
| Parsing | Tokenizes words, quotes, and shell operators |
| Expansions | Processes environment-variable references and quote-sensitive input |
| Built-ins | `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit` |
| External commands | Resolves executables and launches commands through Unix process APIs |
| Redirections | Input `<`, output `>`, append `>>`, and here-document `<<` |
| Pipelines | Connects multiple commands using `|` |
| Signals | Installs handlers for interactive and here-document behavior |
| Environment | Stores and updates shell variables and tracks exit status |

These capabilities are documented from the repository's implementation; exact behavior and edge-case compatibility should be checked with tests.

## Resources

### Shell Execution Flow

A shell is more than a program that calls `execve()`. It must first understand what the user typed and prepare the process environment.

```text
User types a command
         |
         v
  readline() input
         |
         v
  Validate + tokenize
         |
         v
  Expand variables and
  process shell quoting
         |
         v
  Inspect pipes and
  input/output redirections
         |
         v
  Built-in or external command?
       /                \
      v                  v
 Built-in handler    fork() / execve()
       \                /
        v              v
    Update exit status
         |
         v
      Next prompt
```

The main interactive loop and the top-level command dispatcher are in [`main.c`](main.c). Parsing and execution may take different paths for pipeline and non-pipeline commands.

### 1. Parsing and Expansion

A command such as:

```bash
echo "$USER" > result.txt
```

contains several distinct parts:

| Input | Interpretation |
|---|---|
| `echo` | Command name |
| `"$USER"` | Quoted argument that may require variable expansion |
| `>` | Output-redirection operator |
| `result.txt` | Destination filename |

The lexer/tokenizer represents the input as tokens. Expansion then processes variable references and quote-related behavior before execution. This separation prevents operators from being treated as ordinary command arguments.

Relevant files: [`parsing/tokenizer.c`](parsing/tokenizer.c), [`parsing/expanding.c`](parsing/expanding.c), and [`parsing/check_errors.c`](parsing/check_errors.c).

### 2. Processes: `fork()`, `execve()`, and `waitpid()`

Unix shells commonly use three system calls to run external commands:

| Function | Purpose |
|---|---|
| `fork()` | Create a child process |
| `execve()` | Replace the child process image with an executable |
| `waitpid()` | Wait for a child and collect its termination status |

Some built-ins, especially those that modify the shell's own state (such as `cd` or `export`), need to run in the shell process rather than only in a child.

Relevant files: [`exec/exec_command.c`](exec/exec_command.c), [`exec/exec_builtins.c`](exec/exec_builtins.c), and [`main.c`](main.c).

### 3. Pipes and File Descriptors

A **file descriptor** is an integer referring to an open input/output resource. Standard input is `0` and standard output is `1`.

For a pipeline:

```bash
ls -l | wc -l
```

the shell connects the output of one command to the input of the next:

```text
    ls -l                 kernel pipe                 wc -l
+------------+        +------------------+       +------------+
| stdout (1) +------->| buffered bytes   +------>| stdin (0)  |
+------------+        +------------------+       +------------+
    child A                                        child B
```

The key calls are `pipe()` (create the connection), `dup2()` (redirect a process's input/output), and `close()` (release unused descriptors). Correctly closing unused pipe ends helps avoid processes waiting indefinitely.

Relevant files: [`exec/pipes.c`](exec/pipes.c), [`exec/pipes_init.c`](exec/pipes_init.c), and [`exec/pipes_exec.c`](exec/pipes_exec.c).

### 4. Redirections and Here-Documents

| Operator | Effect |
|---|---|
| `< file` | Read standard input from a file |
| `> file` | Write output to a file, truncating it |
| `>> file` | Append output to a file |
| `<< DELIMITER` | Read input until a delimiter is entered |

For output redirection, the shell opens the destination and uses `dup2()` to associate it with standard output. A here-document collects input until its delimiter and supplies that text to a command.

Relevant files: [`exec/execute_redirections.c`](exec/execute_redirections.c), [`exec/heredoc.c`](exec/heredoc.c), and [`exec/heredoc_expansion.c`](exec/heredoc_expansion.c).

### 5. Environment, Signals, and Memory

- **Environment variables:** the project maintains a linked-list representation of shell variables; built-ins such as `export` and `unset` modify this state.
- **Exit status:** the shell tracks command results so later commands can inspect the status.
- **Signals:** interactive `Ctrl-C` and `Ctrl-\` behavior requires context-sensitive handling, particularly while waiting for child processes or reading a here-document.
- **Memory:** a garbage-collector-style allocation list and explicit cleanup functions help manage allocations created during parsing and command execution.

Relevant files: [`env_var/`](env_var/), [`exec/signals_setup.c`](exec/signals_setup.c), and [`parsing/garbage_collector.c`](parsing/garbage_collector.c).

### Code Organization

```text
Minishell/
├── main.c           Interactive prompt and top-level flow
├── Makefile         Build rules and library linkage
├── includes/        Shared structures and declarations
├── parsing/         Tokenization, expansions, validation, cleanup
├── builtins/        Shell built-in commands
├── exec/            Processes, redirections, pipes, here-documents, signals
├── env_var/         Environment-variable management
└── libft/           Supporting C library
```

### References

- [`fork(2)` — Linux manual](https://man7.org/linux/man-pages/man2/fork.2.html)
- [`execve(2)` — Linux manual](https://man7.org/linux/man-pages/man2/execve.2.html)
- [`pipe(2)` — Linux manual](https://man7.org/linux/man-pages/man2/pipe.2.html)
- [`dup2(2)` — Linux manual](https://man7.org/linux/man-pages/man2/dup.2.html)
- [GNU Readline documentation](https://tiswww.case.edu/php/chet/readline/rltop.html)

## Contributors

A collaborative **42 Minishell** project by **hsennane** ([@hajarsen](https://github.com/hajarsen)) and **sel-jari** ([@0xseljarida](https://github.com/0xseljarida)).

The repository and its Git history should retain attribution for each contributor's work.
