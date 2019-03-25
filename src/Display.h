#ifndef DISPLAY_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H


struct coordinates {
  int x = 1;
  int y = 1;
};

struct pixelInfo {
  int totalAmountPixW;
  int totalAmountPixB;
  int totalAmountPixR;
  coordinates coords;
};

pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo);

coordinates draw2Byte(int amountPixW, int amountPixB, int amountPixR, coordinates coords);

void gfxInit();

void gfxClearBuffer();

void gfxCommitBuffer();

void gfxSetRotation(int rot);

void gfxDemo();

#endif