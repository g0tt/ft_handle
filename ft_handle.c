#include "ftd2xx.h"
#include "spi.c"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

int main() {
  FT_HANDLE ftHandle[4];
  FT_STATUS ftStatus;
  DWORD numDevs;
  DWORD devIndex = 0;
  DWORD WriteNum, TransNum;
  int devcnt = 0, devlast;
  unsigned char buf[256];

  printf("Init\n");
  ftStatus = FT_ListDevices(&numDevs, NULL, FT_LIST_NUMBER_ONLY);
  if (ftStatus != 0) exit(1);
  ftStatus = FT_ListDevices((PVOID)devIndex, buf, FT_LIST_BY_INDEX | FT_OPEN_BY_SERIAL_NUMBER);
  if (ftStatus != 0) exit(1);

  printf("Open\n");
  ftStatus = FT_OpenEx(buf, FT_OPEN_BY_SERIAL_NUMBER, &ftHandle[devcnt]);
  if (ftStatus != 0) exit(1);

  printf("BitMode\n");
  ftStatus = FT_SetBitMode(ftHandle[devcnt], 0x00, FT_BITMODE_RESET);
  if (ftStatus != 0) exit(1);
  ftStatus = FT_SetBitMode(ftHandle[devcnt], 0xff, FT_BITMODE_ASYNC_BITBANG);
  if (ftStatus != 0) exit(1);
  ftStatus = FT_SetBaudRate(ftHandle[devcnt], 115200);
  if (ftStatus != 0) exit(1);
  printf("BitMode set end\n");

  /*
  // LED往復
  int data = 1;
  int mode = 0;
  while(1) {
    buf[0] = data;
    WriteNum = 1;
    ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);
    if (mode == 0) {
      data <<= 1;
      if (data == 128) mode = 1;
    } else {
      data >>= 1;
      if (data == 1) mode = 0;
    }

    usleep(30000);
  }
  */

  WriteNum = SPI_Initialize(buf);
  ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);
  if (ftStatus != 0) exit(1);

  while(1) {
    int data;
    printf("Input> ");
    scanf("%d", &data);
    WriteNum = SPI_WriteBuf(buf, data);
    ftStatus = FT_Write(ftHandle[devcnt], buf, WriteNum, &TransNum);
    if (ftStatus != 0) exit(1);
  }

  return 0;
}
