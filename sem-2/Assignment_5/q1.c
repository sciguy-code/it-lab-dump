#include <stdio.h>
#include <stdlib.h>

int main (int argc, char *argv[]){
  
  const char *fn = argc > 1 ? argv[1] : "a.out";
  FILE *fp = fopen(fn, "rb");
  
  if (!fp) { perror("fopen!!"); exit(1); }
  
  fseek(fp, 0, SEEK_END);
  long sz = ftell(fp);
  rewind(fp);
  printf("File: %s | Size: %ld bytes\n\n", fn, sz);
  
  unsigned char buffer[16];
  long off = 0;
  size_t n;
  
  while ((n = fread(buffer, 1, 16, fp)) > 0){
    printf("%08lx ", off);
    
    for (int i = 0; i < 16; i++){
      if (i < n) printf("%02x", buffer[i]);
      else printf("  ");
    }
    printf("  |");
    
    for (int i = 0; i < n; i++){
      printf("%c", (buffer[i] >= 32 && buffer[i] < 127 ? buffer[i] : '.'));
    }
    
    printf("|\n");
    off += n;
  
  }
  if (ferror(fp)) perror("fread!!");
  fclose(fp);

  return 0;

}
