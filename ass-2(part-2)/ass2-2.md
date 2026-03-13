# Assignment 2 — File I/O and Standard I/O Library

**Course:** IT Lab  
**Objective:** Understand UNIX file I/O by implementing file operations first with the standard I/O library, then with raw system calls (`open`, `read`, `write`, `lseek`, `close`).

---

## File Structure

```
ass-itlab/
├── myheader.h    — Custom header file (Advanced)
├── part1a.c      — Std I/O: write to file from console
├── part1b.c      — Std I/O: read from file, print to console
├── part2a.c      — Syscalls: copy stdin to stdout
├── part2b.c      — Syscalls: print file to stdout
├── part2c.c      — Syscalls: write console input to file
├── part2d.c      — Syscalls: read file, print to console
└── README.md
```

---

## Compilation and Usage

```bash
gcc -Wall -o part1a part1a.c
gcc -Wall -o part1b part1b.c
gcc -Wall -o part2a part2a.c
gcc -Wall -o part2b part2b.c
gcc -Wall -o part2c part2c.c
gcc -Wall -o part2d part2d.c
```

Programs that operate on files take a filename as a command-line argument:

```bash
./part1a myfile.txt       # write to myfile.txt from console
./part1b myfile.txt       # read myfile.txt and print contents
```

`part2a` takes no arguments — it simply copies stdin to stdout:

```bash
./part2a
```

Press `Ctrl+D` to signal end-of-input (EOF).

---

## Part 1 — Standard I/O Library

Uses `fopen`, `fgets`, `fputs`, `fclose`, and `ferror`.

### part1a.c — Write to file

1. Opens the given file in `"w"` mode (creates if it does not exist, truncates if it does).
2. Reads lines from stdin using `fgets()`.
3. Writes each line to the file using `fputs()`.
4. Checks for errors with `ferror()` and closes the file.

### part1b.c — Read from file

1. Opens the given file in `"r"` mode.
2. Reads line by line with `fgets()` and prints to stdout.
3. Checks for read errors, closes the file.

---

## Part 2 — Without Standard I/O (Raw System Calls)

Uses `open()`, `read()`, `write()`, `close()` from `<unistd.h>` and `<fcntl.h>`. No `scanf`, `printf`, `fgets`, or `fputs`.

Instead of `FILE *` pointers, these programs work with integer file descriptors. The standard file descriptors are `STDIN_FILENO` (0), `STDOUT_FILENO` (1), and `STDERR_FILENO` (2).

### part2a.c — Copy stdin to stdout

Reads raw bytes from `STDIN_FILENO` and writes them to `STDOUT_FILENO` in a loop.

### part2b.c — Print file contents to stdout

Opens a file with `open(filename, O_RDONLY)`, reads into a buffer, writes to `STDOUT_FILENO`.

### part2c.c — Write console input to file

Opens/creates a file using `open(filename, O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS)`, reads from `STDIN_FILENO`, and writes to the file descriptor.

### part2d.c — Read file, print to console

Opens the file read-only, reads into a buffer, writes to stdout. Same approach as `part2b`.

---

## Advanced — Custom Header and Error Handling

### myheader.h

A single reusable header file that bundles everything needed across all programs.

**System headers included:**

| Header | Purpose |
|--------|---------|
| `<stdio.h>` | Standard I/O functions (Part 1) |
| `<stdlib.h>` | `exit()`, `EXIT_FAILURE` |
| `<string.h>` | `strlen()` for measuring message lengths |
| `<unistd.h>` | `read()`, `write()`, `close()`, file descriptor constants |
| `<fcntl.h>` | `open()`, flags like `O_RDONLY`, `O_WRONLY`, `O_CREAT` |
| `<sys/types.h>` | System data types (`ssize_t`, etc.) |
| `<sys/stat.h>` | File permission macros (`S_IRUSR`, `S_IWUSR`, etc.) |
| `<errno.h>` | Error number variable set by failed system calls |

**Global constants:**

- `BUFSIZE` (1024) — buffer size for all read/write operations.
- `FILE_PERMS` (`S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH`, i.e. `rw-r--r--`) — default permissions for newly created files.

**Error handling function:**

```c
void err_exit(const char *msg)
{
    perror(msg);
    exit(EXIT_FAILURE);
}
```

`perror()` reads the global `errno` variable (set automatically when a system call fails) and prints the caller's message followed by the system error description. For example, trying to open a nonexistent file produces:

```
open: No such file or directory
```

Every program calls `err_exit()` on failure, so error reporting is consistent across all files.

**Include guard** (`#ifndef MYHEADER_H ... #endif`) prevents double inclusion.

---

## Standard I/O vs System Calls

| Aspect | Part 1 (Std I/O) | Part 2 (System Calls) |
|--------|-------------------|----------------------|
| Header | `<stdio.h>` | `<unistd.h>`, `<fcntl.h>` |
| File handle | `FILE *fp` | `int fd` |
| Open | `fopen("file", "w")` | `open("file", O_WRONLY \| O_CREAT, perms)` |
| Read | `fgets(buf, size, fp)` | `read(fd, buf, size)` |
| Write | `fputs(buf, fp)` | `write(fd, buf, n)` |
| Close | `fclose(fp)` | `close(fd)` |
| Buffering | Automatic | None (raw bytes) |
| Error check | `ferror(fp)` | Return value check (`-1` on error) |

---

## System Calls Reference

```c
int open(const char *pathname, int flags, mode_t mode);
// Returns file descriptor on success, -1 on error

ssize_t read(int fd, void *buf, size_t count);
// Returns bytes read, 0 on EOF, -1 on error

ssize_t write(int fd, const void *buf, size_t count);
// Returns bytes written, -1 on error

int close(int fd);
// Returns 0 on success, -1 on error

off_t lseek(int fd, off_t offset, int whence);
// Returns new offset on success, -1 on error
```
