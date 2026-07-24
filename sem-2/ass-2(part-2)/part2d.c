/*
 * Part 2d: Read from file and print to console without fputs
 * Same as 2b basically but this is the "read and print" version
 * Uses open(), read(), write(), close() only
 * 
 * Usage: ./part2d <filename>
 */

#include "myheader.h"

int main(int argc, char *argv[])
{
    int fd;
    ssize_t n;
    char buf[BUFSIZE];

    if (argc != 2) {
        const char *usage = "Usage: ./part2d <filename>\n";
        write(STDERR_FILENO, usage, strlen(usage));
        exit(EXIT_FAILURE);
    }

    // open file for reading
    fd = open(argv[1], O_RDONLY);
    if (fd < 0)
        err_exit("open");

    const char *header = "File contents:\n";
    write(STDOUT_FILENO, header, strlen(header));

    // read from file, dump to stdout
    while ((n = read(fd, buf, BUFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n)
            err_exit("write to stdout");
    }

    if (n < 0)
        err_exit("read");

    if (close(fd) < 0)
        err_exit("close");

    return 0;
}
