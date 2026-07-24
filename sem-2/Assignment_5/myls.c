#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include "myerr.h"

void ls(const char *path){
  DIR *d = opendir(path);
  if (!d) err_exit(path);
  
  struct dirent *e;
  printf ("Contents of the File: %s\n", path);
  printf("____________________________________________________\n\n");
  
  while ((e = readdir(d)) != NULL) {
    if (strcmp(e->d_name, ".") == 0 || strcmp(e->d_name, "..") == 0) continue;
    
    if (e->d_type == DT_DIR){
      printf("%s/\n", e->d_name);
    }
    else if (e->d_type == DT_LNK){
      printf("%s@\n", e->d_name);
    }
    
    else{
      printf("%s\n", e->d_name);
    }
  }
  printf("\n____________________________________________________\n");
  closedir(d);
  
}


int main(int argc, const char *argv[]){
  if (argc == 1) ls(".");
  else if (argc == 2) ls(argv[1]);
  
  else {
    err_msg("Usage : ./myls [dir]");
  }

  return 0;
}
