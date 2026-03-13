/*
 * Part 2c: Write to file from console without fgets
 * Uses open(), read(), write(), close() — no standard I/O functions
 * 
 * Usage: ./part2c <filename>
 */

#include "myheader.h"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t n;
    char buf[BUFSIZE];

    if (argc != 2) {
        const char *usage = "Usage: ./part2c <filename>\n";
        write(STDERR_FILENO, usage, strlen(usage));
        exit(EXIT_FAILURE);
    }

    /*
     * O_WRONLY - open for writing only
     * O_CREAT  - create the file if it doesn't exist
     * O_TRUNC  - if file exists, truncate it (start fresh)
     * FILE_PERMS - permissions rw-r--r-- (defined in myheader.h)
     */
    fd = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, FILE_PERMS);
    if (fd < 0)
        err_exit("open");

    const char *prompt = "Enter text (press Ctrl+D to stop):\n";
    write(STDOUT_FILENO, prompt, strlen(prompt));

    // read from keyboard (stdin) and write to the file
    while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0) {
        if (write(fd, buf, n) != n)
            err_exit("write");
    }

    if (n < 0)
        err_exit("read");

    if (close(fd) < 0)
        err_exit("close");

    const char *done = "File written successfully.\n";
    write(STDOUT_FILENO, done, strlen(done));

    return 0;
}
