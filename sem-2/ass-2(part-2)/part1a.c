/*
 * Part 1a: Using standard I/O library
 * Open file, write from console, close file, report errors
 * 
 * Usage: ./part1a <filename>
 */

#include "myheader.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[BUFSIZE];

    // check if filename is given
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open file in write mode (creates if doesn't exist)
    fp = fopen(argv[1], "w");
    if (fp == NULL)
        err_exit("fopen (write)");

    printf("Enter text (press Ctrl+D to stop):\n");

    // keep reading from console and writing to file
    while (fgets(buf, BUFSIZE, stdin) != NULL) {
        if (fputs(buf, fp) == EOF)
            err_exit("fputs");
    }

    // check if fgets stopped because of an error (not just EOF)
    if (ferror(stdin))
        err_exit("fgets (stdin)");

    // close the file
    if (fclose(fp) != 0)
        err_exit("fclose");

    printf("\nFile '%s' written successfully.\n", argv[1]);

    return 0;
}
