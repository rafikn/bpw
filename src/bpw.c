#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>

#include "../inc/bpw_struct.h"
#include "../inc/bpw_io.h"

static const char * FILENAME = "files/test_1e%d_1e%d.bpw";
static const int FILES = 6;
static const uint64_t TOTAL = 1e8;
static const uint64_t TOTAL_EXP = 8;


void write_files() {
  uint64_t width[6] = {1e3, 1e4, 1e5, 1e6, 1e7, 1e8};
  for (int i = 0; i < FILES; i++) {
    int levels = TOTAL / width[i];
    char filename[24];
    sprintf(filename, FILENAME, i + 3 ,  TOTAL_EXP);

    write_file(filename, width[i], TOTAL, width[i], width[i]);
  }
}

void exec(Byte8 width) {
  char filename[24];
  sprintf(filename, FILENAME, width , TOTAL_EXP);
  read_file(filename);
}


int main(int argc, char **argv) {
  srand(time(NULL));

  if (argc < 2) {
      printf("bpw [cmd]: no command\n");
      printf("possible cmd :\n");
      printf("    gen : generates bpw files\n");
      printf("    eval $w : evaluate file of width w (power of 10): {3, 4, 5, 6}\n");
      return 0;
  }

  if (strcmp("gen", argv[1]) == 0) {
    write_files();
    return 0;
  } else if (strcmp("eval", argv[1]) == 0) {
      if (argc < 3) {
        printf("bpw eval: no width specified\n");
        printf("      w (power of 10): {3, 4, 5, 6}\n");
        return 0;
      }

    Byte8 width = strtoumax(argv[2], NULL, 10);
    printf("Evaluating file with width 1e%d\n", width);
    exec(width);
    printf("done\n");
    return 0;
  }
}
