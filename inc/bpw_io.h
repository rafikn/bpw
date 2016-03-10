#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/*
 * WRITE methods
 */

void write_header(FILE * fp);
void write_bytes(FILE * fp, Byte8 val);
Byte8 generate_bytes(Byte8 limit);
Byte gate_inputs(Byte gate);
Byte generate_gate();
void write_file(char * filename, Byte8 width, Byte8 gates, Byte8 inputs, Byte8 outputs);

/*
 * READ methods
 */
Byte * buffer_file(Byte * path);
Byte * read_header(Byte * buffer);
Byte read_byte(Byte * buffer);
Byte8 read_bytes(Byte * buffer);
Bit * init_input(Byte8 width);
Level * init_level(Byte8 w);
Level * read_level(Byte8 w, Byte * buffer);
Byte eval(Bit * inputs, Gate gate);
void read_file(char * filename);
