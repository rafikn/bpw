#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void write_header(FILE * fp) {
  putc(0x42, fp);
  putc(0x57, fp);
  putc(0x50, fp);
  putc(0x01, fp);
}

void write_uint64_t(FILE * fp, uint64_t val) {
  putc((val >>  0) & 0xff, fp);
  putc((val >>  8) & 0xff, fp);
  putc((val >> 16) & 0xff, fp);
  putc((val >> 24) & 0xff, fp);
  putc((val >> 32) & 0xff, fp);
  putc((val >> 40) & 0xff, fp);
  putc((val >> 48) & 0xff, fp);
  putc((val >> 56) & 0xff, fp);
}

char * buffer_file(char * path) {
  FILE *fileptr;
  char *buffer;
  long filelen;

  fileptr = fopen(path, "rb");  // Open the file in binary mode
  fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
  filelen = ftell(fileptr);             // Get the current byte offset in the file
  rewind(fileptr);                      // Jump back to the beginning of the file

  buffer = (char *)malloc((filelen+1)*sizeof(char)); // Enough memory for file + \0
  fread(buffer, filelen, 1, fileptr);
  fclose(fileptr);

  return buffer;
}

char * read_header(char * buffer) {
  buffer++;
  buffer++;
  buffer++;
  buffer++;
}

uint64_t read_uint64_t(char * buffer) {
  return  ((uint64_t) *(buffer++) <<  0) |
          ((uint64_t) *(buffer++) <<  8) |
          ((uint64_t) *(buffer++) << 16) |
          ((uint64_t) *(buffer++) << 24) |
          ((uint64_t) *(buffer++) << 32) |
          ((uint64_t) *(buffer++) << 40) |
          ((uint64_t) *(buffer++) << 48) |
          ((uint64_t) *(buffer++) << 56);
}
