#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "bpm_struct.h"

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

Byte * buffer_file(Byte * path) {
  FILE *fileptr;
  Byte *buffer;
  long filelen;

  fileptr = fopen(path, "rb");  // Open the file in binary mode
  fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
  filelen = ftell(fileptr);             // Get the current byte offset in the file
  rewind(fileptr);                      // Jump back to the beginning of the file

  buffer = (Byte *)malloc((filelen+1)*sizeof(Byte)); // Enough memory for file + \0
  fread(buffer, filelen, 1, fileptr);
  fclose(fileptr);

  return buffer;
}

Byte * read_header(Byte * buffer) {
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%d\n", *buffer++);
  return buffer;
}

Byte * move_8_bytes(Byte * buffer) {
    for (int i = 0; i < 8;i++) buffer++;
    return buffer;
}

uint64_t read_uint64_t(Byte * buffer) {
  return  ((uint64_t) *(buffer++) <<  0) |
          ((uint64_t) *(buffer++) <<  8) |
          ((uint64_t) *(buffer++) << 16) |
          ((uint64_t) *(buffer++) << 24) |
          ((uint64_t) *(buffer++) << 32) |
          ((uint64_t) *(buffer++) << 40) |
          ((uint64_t) *(buffer++) << 48) |
          ((uint64_t) *(buffer++) << 56);
}

Byte read_byte(Byte * buffer) {
  return * buffer++;
}


uint64_t generate_uint64_t(uint64_t limit) {

  uint64_t num;

  num = rand();
  num = (num << 32) | rand();

  return num % limit;
}

Byte gate_inputs(Byte gate) {
  switch (gate) {
    case 0x00: // not
      return 0x01;
      break;
    case 0x01: //and2
    case 0x02: //or2
    case 0x03: // xor2
    case 0x04: //nand2
    case 0x05: //nor2
    case 0x06: //xnor2
      return 0x02;
      break;
    case 0x07: //and3
    case 0x08: //or3
    case 0x09: //nand3
    case 0x0a: //nor3
    case 0x0b: //xor3
    case 0x0c: //xnor3
      return 3;
      break;
    case 0x0d: //mux3
    case 0x0e:
    case 0x0f:
      return 0;
      break;
  }
}

Byte eval(Bit * inputs, Gate gate) {
  switch (gate.type) {
    case 0x00: // not
      return !inputs[*gate.inputs];
    case 0x01: //and2
      return inputs[*gate.inputs++] & inputs[*gate.inputs++];
    case 0x02: //or2
      return inputs[*gate.inputs++] | inputs[*gate.inputs++];
    case 0x03: //xor2
        return inputs[*gate.inputs++] ^ inputs[*gate.inputs++];
    case 0x04: //nand2
      return !(inputs[*gate.inputs++] & inputs[*gate.inputs++]);
    case 0x05: //nor2
      return !(inputs[*gate.inputs++] | inputs[*gate.inputs++]);
    case 0x06: //xnor2
      return !(inputs[*gate.inputs++] ^ inputs[*gate.inputs++]);
    case 0x07: //and3
      return inputs[*gate.inputs++] & inputs[*gate.inputs++] & inputs[*gate.inputs++];
    case 0x08: //or3
      return inputs[*gate.inputs++] | inputs[*gate.inputs++] | inputs[*gate.inputs++];
    case 0x09: //nand3
      return !(inputs[*gate.inputs++] & inputs[*gate.inputs++] & inputs[*gate.inputs++]);
    case 0x0a: //nor3
      return !(inputs[*gate.inputs++] | inputs[*gate.inputs++] | inputs[*gate.inputs++]);
    case 0x0b: //xor3
      return inputs[*gate.inputs++] ^ inputs[*gate.inputs++] ^ inputs[*gate.inputs++];
    case 0x0c: //xnor3
      return !(inputs[*gate.inputs++] ^ inputs[*gate.inputs++] ^ inputs[*gate.inputs++]);
    case 0x0d: //mux3
      if (inputs[*gate.inputs++]) return inputs[*gate.inputs++]; else return inputs[*(++gate.inputs)];
    case 0x0e: //copy
    case 0x0f:
      printf("undefined\n");
      break;
  }
}

Byte generate_gate() {
  while (1) {
    Byte gate = (rand()&1 << 0 |
      rand()&1 << 1 |
      rand()&1 << 2 |
      rand()&1 << 3
    ) & (0xff >> 4);
    if (gate_inputs(gate)) return gate;
  }
}
