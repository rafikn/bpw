#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>


#include "IO.h"

int main() {

  FILE * fp;
  uint64_t width_w, gates_w, inputs_w, outputs_W;
  uint64_t width_r, gates_r, inputs_r, outputs_r;

  fp = fopen("test.bpm", "wb");

  width_w = (uint64_t) 4;
  gates_w = (uint64_t) 8;
  inputs_w = (uint64_t) 16;
  outputs_W = (uint64_t) 8;

  write_header(fp);
  write_uint64_t(fp, width_w);
  write_uint64_t(fp, gates_w);
  write_uint64_t(fp, inputs_w);
  write_uint64_t(fp, outputs_W);



  fclose(fp);

  char * buffer = buffer_file("test.bpm");

  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%d\n", *buffer++);

  width_r = read_uint64_t(buffer);
  for (int i =0; i < 8;i++) buffer++;

  gates_r = read_uint64_t(buffer);
  for (int i =0; i < 8;i++) buffer++;

  inputs_r = read_uint64_t(buffer);
  for (int i =0; i < 8;i++) buffer++;

  outputs_r = read_uint64_t(buffer);
  for (int i =0; i < 8;i++) buffer++;


  printf("width=%llu\n", width_r);
  printf("gates=%llu\n", gates_r);
  printf("inputs_r=%llu\n", inputs_r);
  printf("outputs_r=%llu\n", gates_r);

  return 0;
}
