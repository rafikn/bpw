#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>

#include "bpm_io.h"

void write(uint64_t width, uint64_t gates, uint64_t inputs, uint64_t outputs) {
    FILE * fp;
    fp = fopen("test.bpm", "wb");

    write_header(fp);
    write_uint64_t(fp, width);
    write_uint64_t(fp, gates);
    write_uint64_t(fp, inputs);
    write_uint64_t(fp, outputs);

    for (int i = 0; i < gates; i++) {
      char gate = -1;

      while (gate == -1) gate = generate_gate();

      putc(gate, fp);

      printf("generated gate = %02x\n", gate);

      uint64_t a = 0;
      uint64_t b = 0;
      uint64_t c = 0;
      switch (gate_inputs(gate)) {
        case 1:
          // generate 1 random input
          a = generate_uint64_t(inputs);
          write_uint64_t(fp, a);
          printf("1 input %llu\n", a);
          break;
        case 2:
          // generate random input * 2
          a = generate_uint64_t(inputs);
          b = generate_uint64_t(inputs);
          write_uint64_t(fp, a);
          write_uint64_t(fp, b);
          printf("2 inputs %llu, %llu \n", a, b );
          break;
        case 3:
          // generate random input * 3
          a = generate_uint64_t(inputs);
          b = generate_uint64_t(inputs);
          c = generate_uint64_t(inputs);
          write_uint64_t(fp, a);
          write_uint64_t(fp, b);
          write_uint64_t(fp, c);
          printf("3 inputs %llu, %llu, %llu \n", a,b,c);
      }

    }
    fclose(fp);
}

void read() {
  uint64_t width, gates, inputs, outputs;

  char * buffer = buffer_file("test.bpm");

  /* read the first 4 bytes*/
  buffer = read_header(buffer);

  /* read the next 8 bytes*/
  width = read_uint64_t(buffer);
  buffer = move_8_bytes(buffer);

  /* read the next 8 bytes*/
  gates = read_uint64_t(buffer);
  buffer = move_8_bytes(buffer);

  /* read the next 8 bytes*/
  inputs = read_uint64_t(buffer);
  buffer = move_8_bytes(buffer);

  /* read the next 8 bytes*/
  outputs = read_uint64_t(buffer);
  buffer = move_8_bytes(buffer);


  printf("width=%llu\n", width);
  printf("gates=%llu\n", gates);
  printf("inputs_r=%llu\n", inputs);
  printf("outputs_r=%llu\n", outputs);


  for (int i = 0; i < gates; i++) {
      char gate = read_char(buffer);
      printf("gate_%d type=%02x\n", i, gate);
      buffer++;

      for (int j = 0; j < gate_inputs(gate); j++) {
        printf("gate_%d input_%d=%llu\n", i, j, read_uint64_t(buffer));
        buffer = move_8_bytes(buffer);
      }

  }
}

int main() {
  srand(time(NULL));

  write(4, 16, 10, 4);
  read();

  return 0;
}
