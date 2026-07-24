/*
 * myheader.h - My custom header file for IT Lab assignments
 * This header includes all the system headers, constants and
 * function prototypes that I'll be using throughout the semester.
 * 
 * Author: Sagnik Biswas
 */

#ifndef MYHEADER_H
#define MYHEADER_H

/* --- System headers --- */
#include <stdio.h>       // for standard I/O stuff (fopen, fgets, fputs, etc.)
#include <stdlib.h>      // for exit(), malloc(), etc.
#include <string.h>      // for string functions like strlen, strcpy
#include <unistd.h>      // for read(), write(), close() system calls
#include <fcntl.h>       // for open() and file control options like O_RDONLY
#include <sys/types.h>   // for data types used in system calls (ssize_t, etc.)
#include <sys/stat.h>    // for file permission constants like S_IRUSR
#include <errno.h>       // to get error numbers when something goes wrong

/* --- Global constants --- */
#define BUFSIZE 1024     // buffer size for reading/writing
#define FILE_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH)  // rw-r--r-- permissions for new files

/*
 * err_exit - Custom error handling function
 * 
 * This function prints an error message along with the
 * system error description (using errno) and then exits
 * the program with EXIT_FAILURE.
 *
 * msg: a short string telling what went wrong
 */
void err_exit(const char *msg)
{
    /* 
     * perror() prints "msg: <system error description>"
     * so we get both our message and what the OS says
     */
    perror(msg);
    exit(EXIT_FAILURE);
}

#endif /* MYHEADER_H */
