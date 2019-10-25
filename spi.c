#include "ftd.h"

#define CS D2
#define SCK D0
#define SDI D1
#define LDAC D3

void ticktock(unsigned char *buf, int *index, unsigned char *current) {
  *current |= SCK;
  buf[*index] = *current;
  *current &= (~SCK);
  buf[*index + 1] = *current;
  *index += 2;
}

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
    ticktock(buf, &index, &current);
  }

  // Config bit 13-12: 1
  for (int i = 13; i > 11; i--) {
    current |= SDI;
    buf[index++] = current;
    ticktock(buf, &index, &current);
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
    ticktock(buf, &index, &current);
  }

  // Ignored
  for (int i = 1; i >= 0; i--) {
    current &= (~SDI);
    buf[index++] = current;
    ticktock(buf, &index, &current);
  }

  current |= CS;
  buf[index++] = current;
  current &= (~LDAC);
  buf[index++] = current;
  current |= LDAC;
  buf[index++] = current;

  return index;
}
