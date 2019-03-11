#ifndef DISPLAY_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define DISPLAY_H

void refreshScreen(uint8_t* buff);

void setPixels(int anzPixel, int col, int lastX, int lastY);

#endif