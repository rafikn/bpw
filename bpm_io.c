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
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%d\n", *buffer++);
  return buffer;
}

char * move_8_bytes(char * buffer) {
    for (int i = 0; i < 8;i++) buffer++;
    return buffer;
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

char read_char(char * buffer) {
  return * buffer++;
}


uint64_t generate_uint64_t(uint64_t limit) {

  uint64_t num;

  num = rand();
  num = (num << 32) | rand();

  return num % limit;
}

char gate_inputs(char gate) {
  switch (gate) {
    case 0x00: // not
      return 0x01;
      break;
    case 0x01: //and2
    case 0x02: //or2
    case 0x03: //nand2
    case 0x04: //nor2
    case 0x05: //xnor2
      return 0x02;
      break;
    case 0x06: //and3
    case 0x07: //or3
    case 0x08: //nand3
    case 0x09: //nor3
    case 0x0a: //xor3
    case 0x0b: //xnor3
    case 0x0c: //mux3
    case 0x0d: //copy
      return 3;
      break;
    case 0x0e:
    case 0x0f:
      return -1;
      break;
  }
}

char eval(char gate, char * inputs) {
  switch (gate) {
    case 0x00: // not
    case 0x01: //and2
    case 0x02: //or2
    case 0x03: //nand2
    case 0x04: //nor2
    case 0x05: //xnor2
    case 0x06: //and3
    case 0x07: //or3
    case 0x08: //nand3
    case 0x09: //nor3
    case 0x0a: //xor3
    case 0x0b: //xnor3
    case 0x0c: //mux3
    case 0x0d: //copy
      return 3;
      break;
    case 0x0e:
    case 0x0f:
      printf("undefined\n");
      break;
  }
}

char generate_gate() {
  while (1) {
    char gate = (rand()&1 << 0 |
      rand()&1 << 1 |
      rand()&1 << 2 |
      rand()&1 << 3
    ) & (0xff >> 4);
    if (gate_inputs(gate) != -1) return gate;
  }
}
