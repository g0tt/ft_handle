#include <stdint.h>

#define D0 0x1
#define D1 0x2
#define D2 0x4
#define D3 0x8
#define D4 0x10
#define D5 0x20
#define D6 0x40
#define D7 0x80
#define CS D2
#define SCK D0
#define SDI D1
#define LDAC D3

typedef uint8_t uint10_t;

int SPI_Initialize(unsigned char *buf) {
  buf[0] = CS | LDAC;
  return 1;
}

int SPI_WriteBuf(unsigned char *buf, int data) {
  int index = 0;
  unsigned char current;
  current = CS | LDAC;

  // CS Down
  current = current & (~CS);
  buf[index++] = current;

  // Config bit 15-14: 0
  for (int i = 15; i > 13; i--) {
    current &= (~SDI);
    buf[index++] = current;
    current |= SCK;
    buf[index++] = current;
    current &= (~SCK);
    buf[index++] = current;
  }
  // Config bit 13-12: 1
  for (int i = 13; i > 11; i--) {
    current |= SDI;
    buf[index++] = current;
    current |= SCK;
    buf[index++] = current;
    current &= (~SCK);
    buf[index++] = current;
  }

  // Data
  for (int i = 11; i > 1; i--) {
    int data_bit = data & (1 << (i - 2));
    if (data_bit > 0) {
      current |= SDI;
    } else {
      current &= (~SDI);
    }
    buf[index++] = current;
    current |= SCK;
    buf[index++] = current;
    current &= (~SCK);
    buf[index++] = current;
  }

  // Ignored
  for (int i = 1; i >= 0; i--) {
    current &= (~SDI);
    buf[index++] = current;
    current |= SCK;
    buf[index++] = current;
    current &= (~SCK);
    buf[index++] = current;
  }

  current |= CS;
  buf[index++] = current;
  /*
  current |= SCK;
  buf[index++] = current;
  */
  current &= (~LDAC);
  buf[index++] = current;
  /*
  current &= (~SCK);
  buf[index++] = current;
  */
  current |= LDAC;
  buf[index++] = current;
  /*
  current &= (~CS);
  buf[index++] = current;
  */

  return index;
}
