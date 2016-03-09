#include <stdlib.h>
#include <stdint.h>

typedef unsigned char Byte;

typedef struct Gate {
  Byte type;
  uint64_t * inputs;
  uint64_t output;
} Gate;

typedef struct Level {
  Gate * gates;
  Byte * buffer;
  struct Level * next;
} Level;

typedef unsigned char Bit;

#define ON 0x01 (Bit);
#define OFF 0x00 (Bit);
