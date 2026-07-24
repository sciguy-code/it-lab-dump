#ifndef MYERR_H
#define MYERR_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h> 
#include <string.h>

  void err_exit (const char *s){
    fprintf (stderr, "[ERROR!] %s : %s", s, strerror(errno));
    exit(1);
  }
  
  void err_msg (const char *s){
    fprintf (stderr, "[ERROR!] %s\n", s);
    exit(1);
  }

#endif
