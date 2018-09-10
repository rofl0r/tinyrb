#include <stdio.h>
#include <sys/stat.h>

#include "tr.h"
#include "internal.h"

char *read_file_into_string(char *filename, size_t *str_len) {
  FILE *fp;
  struct stat stats;

  *str_len = 0;
  if (stat(filename, &stats) == -1) return NULL;
  fp = fopen(filename, "rb");
  if (!fp) return NULL;
  
  char *string = TR_ALLOC_N(char, stats.st_size + 1);
  size_t bytes_read = fread(string, 1, stats.st_size, fp);
  fclose(fp);
  if (bytes_read == (size_t)stats.st_size) {
    string[stats.st_size] = 0;
    *str_len = stats.st_size;
    return string;
  }
  TR_FREE(string);
  return NULL;
}
