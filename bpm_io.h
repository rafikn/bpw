#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void write_header(FILE * fp);
void write_uint64_t(FILE * fp, uint64_t val);

Byte * buffer_file(Byte * path);
Byte * move_8_bytes(Byte * buffer);

Byte * read_header(Byte * buffer);

Byte read_byte(Byte * buffer);
uint64_t read_uint64_t(Byte * buffer);

Byte generate_gate();
Byte gate_inputs(Byte gate);
Byte * gate_name(Byte gate);
uint64_t generate_uint64_t(uint64_t limit);

Bit eval(Bit * inputs, Gate gate);
