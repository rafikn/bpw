#include <stdlib.h>
#include <stdint.h>

typedef unsigned char Byte;
typedef uint64_t  Byte8;

typedef struct Gate {
  Byte type;
  Byte8 * inputs;
} Gate;

typedef struct Level {
  Gate * gates;
  Byte * buffer;
  struct Level * next;
} Level;

typedef unsigned char Bit;

#define ON 0x01 (Bit);
#define OFF 0x00 (Bit);
