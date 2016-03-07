#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void write_header(FILE * fp);
void write_uint64_t(FILE * fp, uint64_t val);

char * buffer_file(char * path);
char * move_8_bytes(char * buffer);

char * read_header(char * buffer);

char read_char(char * buffer);
uint64_t read_uint64_t(char * buffer);

char generate_gate();
char gate_inputs(char gate);
char * gate_name(char gate);
uint64_t generate_uint64_t(uint64_t limit);
