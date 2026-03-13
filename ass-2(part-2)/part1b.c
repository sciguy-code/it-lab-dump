/*
 * Part 1b: Using standard I/O library
 * Open file, read contents, print to console, report errors
 * 
 * Usage: ./part1b <filename>
 */

#include "myheader.h"

int main(int argc, char *argv[])
{
    FILE *fp;
    char buf[BUFSIZE];

    if (argc != 2) {
        fprintf(stderr, "Usage: %s <filename>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // open file in read mode
    fp = fopen(argv[1], "r");
    if (fp == NULL)
        err_exit("fopen (read)");

    printf("Contents of '%s':\n", argv[1]);

    // read line by line and print to console
    while (fgets(buf, BUFSIZE, fp) != NULL) {
        printf("%s", buf);
    }

    // check if loop ended due to error
    if (ferror(fp))
        err_exit("fgets (file)");

    if (fclose(fp) != 0)
        err_exit("fclose");

    return 0;
}
