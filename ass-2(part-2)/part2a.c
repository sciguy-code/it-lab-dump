/*
 * Part 2a: Without using standard I/O library
 * Copy standard input to standard output using read() and write()
 * No scanf, no printf — just raw system calls
 */

#include "myheader.h"

int main()
{
    char buf[BUFSIZE];
    ssize_t n;  // number of bytes read

    /*
     * STDIN_FILENO = 0 (standard input)
     * STDOUT_FILENO = 1 (standard output)
     * these are defined in <unistd.h>
     */

    // read from stdin, write to stdout, repeat until EOF
    while ((n = read(STDIN_FILENO, buf, BUFSIZE)) > 0) {
        if (write(STDOUT_FILENO, buf, n) != n)
            err_exit("write");
    }

    // if read returned -1, something went wrong
    if (n < 0)
        err_exit("read");

    return 0;
}
