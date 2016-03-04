#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void write_header(FILE * fp);
void write_uint64_t(FILE * fp, uint64_t val);

char * buffer_file(char * path);

char * read_header(char * buffer);
uint64_t read_uint64_t(char * buffer);
