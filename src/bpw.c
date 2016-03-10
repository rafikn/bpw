#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include <inttypes.h>

#include "../inc/bpw_struct.h"
#include "../inc/bpw_io.h"

static const char * FILENAME = "files/test_%d.bwp";


void write_files() {
  int width[11] = {5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000};
  int levels = 100;
  for (int i = 0; i < 11; i++) {
    char filename[24];
    sprintf(filename, FILENAME, width[i]);

    write_file(filename, width[i], width[i] * levels, width[i], width[i]);
  }
}

void exec(Byte8 width) {
  char filename[24];
  sprintf(filename, FILENAME, width);
  read_file(filename);
}


int main(int argc, char **argv) {
  srand(time(NULL));

  if (argc < 2) {
      printf("bpw [cmd]: no command\n");
      printf("possible cmd :\n");
      printf("    gen : generates bpw files\n");
      printf("    eval $w : evaluate file of width w in {5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000}\n");
      return 0;
  }

  if (strcmp("gen", argv[1]) == 0) {
    write_files();
    return 0;
  } else if (strcmp("eval", argv[1]) == 0) {
      if (argc < 3) {
        printf("bpw eval: no width specified\n");
        printf("      w : {5, 10, 50, 100, 500, 1000, 5000, 10000, 50000, 100000, 500000}\n");
        return 0;
      }

    Byte8 width = strtoumax(argv[2], NULL, 10);
    printf("Evaluating file with width \n",width);
    exec(width);
    printf("done");
    return 0;
  }
}
