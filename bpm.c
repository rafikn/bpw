#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>

#include "bpm_io.h"

typedef struct Gate {
  char type;
  uint64_t * inputs;
  uint64_t output;
} Gate;

typedef struct Level {
  Gate * gates;
  char * buffer;
  struct Level * next;
} Level;



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

/**
 * Initialise a level of width w gates
 */
Level * init_level(uint64_t w) {
  Level * root = (Level *) malloc(sizeof(Level));
  root->gates = (Gate *) calloc(w, sizeof(Gate));
  root->next = 0;
  return root;
}

/**
 * Read w gates from buffer
 */
Level * read_level(uint64_t w, char * buffer) {
  Level * level = init_level(w);
  level->gates = (Gate *) calloc(w, sizeof(Gate));
  for (int i = 0; i< w; i++) {
    Gate * g_ptr = (Gate *) malloc(sizeof(Gate));
    // read gate types
    g_ptr->type = read_char(buffer);
    buffer++;
    // read inputs
    g_ptr->inputs = (uint64_t *) calloc(gate_inputs(g_ptr->type), sizeof(uint64_t));
    for (int j = 0; j < gate_inputs(g_ptr->type); j++) {
      g_ptr->inputs[j] = read_uint64_t(buffer);
      buffer += 8;
    }

    level->gates[i] = *g_ptr;
    free(g_ptr);
  }

  level->buffer = buffer;
  return level;
}

/**
 * Read a bpm file
 */
void read() {
  uint64_t width, gates, inputs, outputs;

  char * buffer = buffer_file("test.bpm");

  /* read the first 4 bytes*/
  buffer = read_header(buffer);
  /* read the next 8 bytes*/
  width = read_uint64_t(buffer);
  buffer += 8;
  /* read the next 8 bytes*/
  gates = read_uint64_t(buffer);
  buffer += 8;
  /* read the next 8 bytes*/
  inputs = read_uint64_t(buffer);
  buffer += 8;
  /* read the next 8 bytes*/
  outputs = read_uint64_t(buffer);
  buffer += 8;

  printf("width=%llu\n", width);
  printf("gates=%llu\n", gates);
  printf("inputs_r=%llu\n", inputs);
  printf("outputs_r=%llu\n", outputs);


  int levels = gates / width;
  // initialize first level gate array
  Level * root = read_level(width, buffer);
  levels--;

  Level * level_pre = root;
  while (levels > 0) {
    Level * level_current = read_level(width, level_pre->buffer);
    level_pre->next = level_current;
    level_pre = level_current;
    levels--;
  }

  Level * p = root;
  while (p != 0) {
    for (int i = 0; i < width; i++) {
      printf("loaded gate %02x\n", p->gates[i].type);
    }
    p = p->next;
  }
}

int main() {
  srand(time(NULL));

  write(4, 16, 10, 4);
  read();

  return 0;
}
