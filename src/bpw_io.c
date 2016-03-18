#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "../inc/bpw_struct.h"

/**
 * Write the bpw file header
 * byte 0 = 0x42 (B)
 * byte 1 = 0x57 (w)
 * byte 2 = 0x50 (P)
 * byte 3 = 0x01 (v = 1)
 */
void write_header(FILE * fp) {
  putc(0x42, fp);
  putc(0x57, fp);
  putc(0x50, fp);
  putc(0x01, fp);
}

/**
 * Write 8 bytes tp BPW FILE
 * Use 0 padding
 */
void write_bytes(FILE * fp, Byte8 val) {
  putc((val >>  0) & 0xff, fp);
  putc((val >>  8) & 0xff, fp);
  putc((val >> 16) & 0xff, fp);
  putc((val >> 24) & 0xff, fp);
  putc((val >> 32) & 0xff, fp);
  putc((val >> 40) & 0xff, fp);
  putc((val >> 48) & 0xff, fp);
  putc((val >> 56) & 0xff, fp);
}

/**
 * Generate a random 8-byte (64-bit) number between 0 and limit
 * TODO: Check openssl/rand
 */
Byte8 generate_bytes(Byte8 limit) {
  Byte8 num;
  if( RAND_MAX == 0x7FFFFFFF ) {
    num = rand();
    num = (num << 31) ^ rand(); // a 62-bit signed integer
  } else {
    assert( RAND_MAX == 0x3FFFFFFFFFFFFFFF );
    num = rand(); // a 62-bit signed integer
  }

  if (num % limit > limit) printf("gen: %llu max: %llu\n", num, limit);
  return num % limit;
}

/**
 * Return how many inputs a gate has
 */
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
    case 0x0d: //mux3
      return 3;
      break;
    case 0x0e: // COPY
    case 0x0f:
      return 0;
      break;
  }
}

/**
 * Generate a random gate, i.e choose a random int beween 0x00 and 0x0d
 * TODO: Add COPY
 */
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

/**
 * Create a BPW FILE
 */
void write_file(char * filename, Byte8 width, Byte8 gates, Byte8 inputs, Byte8 outputs) {
    FILE * fp;
    fp = fopen(filename, "wb");

    printf("Creating file %s : [", filename);
    printf("width=%llu ", width);
    printf("gates=%llu ", gates);
    printf("inputs_r=%llu ", inputs);
    printf("outputs_r=%llu]\n", outputs);

    write_header(fp);
    write_bytes(fp, width);
    write_bytes(fp, gates);
    write_bytes(fp, inputs);
    write_bytes(fp, outputs);

    for (int i = 0; i < gates; i++) {
      Byte gate = generate_gate();

      // write gate type
      putc(gate, fp);

      switch (gate_inputs(gate)) {
        case 1:
          // generate 1 random input x 1
          write_bytes(fp, generate_bytes(inputs));
          break;
        case 2:
          // generate random input x 2
          write_bytes(fp, generate_bytes(inputs));
          write_bytes(fp, generate_bytes(inputs));
          break;
        case 3:
          // generate random input x 3
          write_bytes(fp, generate_bytes(inputs));
          write_bytes(fp, generate_bytes(inputs));
          write_bytes(fp, generate_bytes(inputs));
          break;
      }

    }

    printf("OK\n", filename);
    fclose(fp);
}

/**
 * Load a BPW file in a buffer
 * While this would allow to process the file byte by byte,
 * it is not optimal for large BPW files
 */
Byte * buffer_file(Byte * path) {
  FILE *fileptr;
  Byte *buffer;
  long filelen;

  fileptr = fopen(path, "rb");          // Open the file in binary mode
  fseek(fileptr, 0, SEEK_END);          // Jump to the end of the file
  filelen = ftell(fileptr);             // Get the current byte offset in the file
  rewind(fileptr);                      // Jump back to the beginning of the file

  buffer = (Byte *)malloc((filelen+1)*sizeof(Byte)); // Enough memory for file + \0
  fread(buffer, filelen, 1, fileptr);
  fclose(fileptr);

  return buffer;
}

/**
 * Print the first 4 bytes of a BPW file
 * Move the buffer to first gate
 */
Byte * read_header(Byte * buffer) {
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%c", *buffer++);
  printf("%d\n", *buffer++);
  return buffer;
}




/**
 * Execute a gate's logic function
 */
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

Byte eval_gate(Byte gateType, Bit input_1, Bit input_2, Bit input_3) {
  switch (gateType) {
    case 0x00: // not
      return !input_1;
    case 0x01: //and2
      return input_1 & input_2;
    case 0x02: //or2
      return input_1 | input_2;
    case 0x03: //xor2
        return input_1 ^ input_2;
    case 0x04: //nand2
      return !(input_1 & input_2);
    case 0x05: //nor2
      return !(input_1 | input_2);
    case 0x06: //xnor2
      return !(input_1 ^ input_2);
    case 0x07: //and3
      return input_1 & input_2 & input_3;
    case 0x08: //or3
      return input_1 | input_2 | input_3;
    case 0x09: //nand3
      return !(input_1 & input_2 & input_3);
    case 0x0a: //nor3
      return !(input_1 | input_2 | input_3);
    case 0x0b: //xor3
      return input_1 ^ input_2 ^ input_3;
    case 0x0c: //xnor3
      return !(input_1 ^ input_2 ^ input_3);
    case 0x0d: //mux3
      if (input_1) return input_2; else return input_3;
    case 0x0e: //copy
    case 0x0f:
      printf("undefined\n");
      exit(0);
      break;

  }
}

/**
 * Read one byte (8-bit) value
 * NOTE: Buffer still needs to be moved one byte manually after read
 */
Byte read_byte(Byte * buffer) {
  return * buffer;
}

/**
 * Read 8 bytes (64-bit) value
 * NOTE: Buffer still needs to be moved 8 bytes manually after read
 */
Byte8 read_bytes(Byte * buffer) {
  return  ((Byte8) read_byte(buffer++) <<  0) |
          ((Byte8) read_byte(buffer++) <<  8) |
          ((Byte8) read_byte(buffer++) << 16) |
          ((Byte8) read_byte(buffer++) << 24) |
          ((Byte8) read_byte(buffer++) << 32) |
          ((Byte8) read_byte(buffer++) << 40) |
          ((Byte8) read_byte(buffer++) << 48) |
          ((Byte8) read_byte(buffer++) << 56);
}

/**
 * Prepare an array of w-length inputs
 */
Bit * init_input(Byte8 width) {
  Bit * inputs = (Bit * ) calloc(width, sizeof(Bit));
  for (int i = 0; i< width; i++) {
    inputs[i] = (Bit)rand()&1;
  }
  return inputs;

}

Bit * init_arr(Bit * in_arr, Byte8 width) {
  for (int i = 0; i< width; i++) {
    in_arr[i] = (Bit)rand()&1;
  }
  return in_arr;
}

/**
 * Initialise a level of width w gates
 */
Level * init_level(Byte8 w) {
  Level * root = (Level *) malloc(sizeof(Level));
  root->gates = (Gate *) calloc(w, sizeof(Gate));
  root->next = 0;
  return root;
}

/**
 * Read w gates from buffer onto one Level
 */
Level * read_level(Byte8 w, Byte * buffer) {
  Level * level = init_level(w);
  level->gates = (Gate *) calloc(w, sizeof(Gate));
  for (int i = 0; i< w; i++) {
    Gate * g_ptr = (Gate *) malloc(sizeof(Gate));

    g_ptr->type = read_byte(buffer); // read gate type
    buffer++;                        // move buffer

    // read inputs
    g_ptr->inputs = (Byte8 *) calloc(gate_inputs(g_ptr->type), sizeof(Byte8));
    for (int j = 0; j < gate_inputs(g_ptr->type); j++) {
      g_ptr->inputs[j] = read_bytes(buffer);
      buffer += 8;
    }

    level->gates[i] = *g_ptr;
    free(g_ptr);
  }

  level->buffer = buffer;
  return level;
}

/**
 * Read a BPW file
 */
void read_file(char * filename) {
  Byte8 width, gates, inputs, outputs, levels;
  FILE * pFile;
  Byte * buffer, * ptr;
  size_t result;
  long lSize;
  Byte gateType;
  Byte8 input_1;
  Byte8 input_2;
  Byte8 input_3;

  printf("Loading file %s\n", filename);

  pFile = fopen (filename , "rb" );
  // obtain file size:
  fseek (pFile , 0 , SEEK_END);
  lSize = ftell (pFile);
  rewind (pFile);

  // allocate memory to contain the whole file:
  buffer = (Byte*) malloc (sizeof(Byte)*lSize);
  if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

  // copy the file into the buffer:
  result = fread (buffer,1,lSize,pFile);
  if (result != lSize) {fputs ("Reading error",stderr); exit (3);}


  ptr = buffer;

  printf("%c", *ptr++);
  printf("%c", *ptr++);
  printf("%c", *ptr++);
  printf("%x\n", *ptr++);

  width = read_bytes(ptr); ptr += 8;
  gates = read_bytes(ptr); ptr += 8;
  inputs = read_bytes(ptr); ptr += 8;
  outputs = read_bytes(ptr); ptr += 8;
  levels = gates/width;

  printf("Gates: %llu\n", gates);
  printf("Width: %llu\n", width);
  printf("Levels: %llu\n", levels);



  Bit * in_arr = (Bit * ) calloc(width, sizeof(Bit));
  Bit * out_arr = (Bit * ) calloc(width, sizeof(Bit));
  for (int i = 0; i < levels; i++) {

      in_arr = out_arr;
      if (!i) {
        in_arr = init_arr(in_arr, width);
      }

      for (int j = 0; j < width; i++) {
        /* read gate */
        gateType = read_byte(ptr); ptr++;
        switch (gate_inputs(gateType)) {
          case 1:
            input_1 = in_arr[read_byte(ptr)]; ptr+=8;
            break;
          case 2:
            input_1 = in_arr[read_byte(ptr)]; ptr+=8;
            input_2 = in_arr[read_byte(ptr)]; ptr+=8;
            break;
          case 3:
            input_1 = in_arr[read_byte(ptr)]; ptr+=8;
            input_2 = in_arr[read_byte(ptr)]; ptr+=8;
            input_3 = in_arr[read_byte(ptr)]; ptr+=8;
            break;
        }

        //printf("level: %d gate: %x inputs: %d 1:%d 2:%d 3:%d \n", i, gateType, gate_inputs(gateType), input_1, input_2, input_3);
        out_arr[j] = eval_gate(gateType, input_1, input_2, input_3);
    }
  }




  /* the whole file is now loaded in the memory buffer. */

  // terminate
  fclose (pFile);
  free (buffer);


  // Byte * buffer = buffer_file(filename);
  //
  // /* read the first 4 bytes*/
  // buffer = read_header(buffer);
  // /* read the next 8 bytes*/
  // width = read_bytes(buffer);
  // buffer += 8;
  // /* read the next 8 bytes*/
  // gates = read_bytes(buffer);
  // buffer += 8;
  // /* read the next 8 bytes*/
  // inputs = read_bytes(buffer);
  // buffer += 8;
  // /* read the next 8 bytes*/
  // outputs = read_bytes(buffer);
  // buffer += 8;
  //
  // printf("Reading file %s : [", filename);
  // printf("width=%llu ", width);
  // printf("gates=%llu ", gates);
  // printf("inputs_r=%llu ", inputs);
  // printf("outputs_r=%llu]\n", outputs);
  //
  //
  // int levels = gates / width;
  // // initialize first level gate array
  // Level * root = read_level(width, buffer);
  // levels--;
  //
  // Level * level_pre = root;
  // while (levels > 0) {
  //   Level * level_current = read_level(width, level_pre->buffer);
  //   level_pre->next = level_current;
  //   level_pre = level_current;
  //   levels--;
  // }
  //
  // Level * p = root;
  // int l = 0;
  //
  //
  // Bit * in_arr = init_input(width);
  // Bit * out_arr = (Bit * ) calloc(width, sizeof(Bit));
  //
  // while (p != 0) {
  //
  //   for (int i = 0; i < width; i++) {
  //     //printf("loaded gate %02x\n", p->gates[i].type);
  //
  //     out_arr[i] = eval(in_arr, p->gates[i]);
  //   }
  //
  //   memcpy(in_arr, out_arr, sizeof(out_arr));
  //   p = p->next;
  // }
  //
  // free(in_arr);
  // free(out_arr);
}
