#ifndef DISPLAY_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H


struct coordinates {
  int x = 0;
  int y = 0;
};

struct accessTiming{
  int timeLastAccess = 0;
  int timeNextAccess = 0;
};

struct pixelInfo {
  int totalAmountPixW = 0;
  int totalAmountPixB = 0;
  int totalAmountPixR = 0;
  int amountBytes = 0;
  coordinates coords;
  accessTiming at;
};

pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo);

coordinates draw2Byte(int amountPixW, int amountPixB, int amountPixR, coordinates coords);

void gfxInit();

void gfxClearBuffer();

void gfxCommitBuffer();

void gfxSetRotation(int rot);

int IntValOf2Byte(uint8_t* buff);

void gfxDemo();

#endif