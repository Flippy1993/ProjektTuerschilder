#include <Arduino.h>
#include "MiniGrafx.h"
#include <SPI.h>
#include "EPD_WaveShare_75.h"
#include "DisplayDriver.h"
#include "display.h"

using namespace std;

struct coordinates_t {
  int x;
  int y;
} coordinates;

#define CS 16
#define RST 4
#define DC 17
#define BUSY 27

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 384
#define BITS_PER_PIXEL 1

uint16_t palette[] = {0, 3, 4}; //black, white, red
//uint16_t palette[] = {0x00, 0x33, 0x44};

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

void refreshScreen(uint8_t* buff){
    byte img[128];
    for(int i = 14; i<=128;i++){
        img[i] = buff[i];
    }
    int lastX = 1;
    int lastY = 1;
    for(int i = 0; i<128;i=i+2){
        byte w = img[i];
        byte sr = img[i+1];
        int anzW = w;
        int anzS = sr << 3;
        int anzR = (sr << 5) >> 5;
        //int anzS = (sr >> 3) & ((1 << 5)-1);
        //int anzR = (((1 << 3) - 1) & (sr >> (6 - 1)));

        // White
        setPixels(anzW, palette[1], lastX, lastY);
        lastX = coordinates.x;
        lastY = coordinates.y;
        // Black
        setPixels(anzW, palette[0], lastX, lastY);
        lastX = coordinates.x;
        lastY = coordinates.y;
        // Red
        setPixels(anzW, palette[2], lastX, lastY);
        lastX = coordinates.x;
        lastY = coordinates.y;
    }

    void setPixels(int anzPixel, int col, int lastX, int lastY){
        int anzPixelMod = 0;
        // Laufe durch Display Spalten
        for(int y = lastY; y <= 384; y++){
            // Laufe durch Display Zeilen
            for(int x = lastX; x <= 640; x++){
                gfx.setColor(col);
                gfx.setPixel(x,y);

                lastX = x;
                if(x== 640)
                    x = 1;
                anzPixelMod++;
                if(anzPixelMod>= anzPixel){
                    break;
                }
            }
            lastY = y;
            anzPixelMod++;
            if(anzPixelMod>= anzPixel){
                break;
            }
        }
        coordinates.x = lastX;
        coordinates.y = lastY;
    }



    	
 
}