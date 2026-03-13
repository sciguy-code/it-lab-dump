/*
 * Part 2b: Print file contents to stdout without using printf
 * Uses open(), read(), write(), close() system calls only
 * 
 * Usage: ./part2b <filename>
 */

#include "myheader.h"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t n;
    char buf[BUFSIZE];

    if (argc != 2) {
        // can't use printf so write error message manually
        const char *usage = "Usage: ./part2b <filename>\n";
        write(STDERR_FILENO, usage, strlen(usage));
        exit(EXIT_FAILURE);
    }

    // open the file for reading only
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        err_exit("open");

    // read from file and write to stdout
    while ((n = read(fd, buf, BUFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n)
            err_exit("write");
    }

    if (n < 0)
        err_exit("read");

    if (close(fd) < 0)
        err_exit("close");

    return 0;
}
