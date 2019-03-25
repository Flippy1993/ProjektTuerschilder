#include <Arduino.h>
#include "MiniGrafx.h"
#include <SPI.h>
#include "EPD_WaveShare_75.h"
#include "DisplayDriver.h"
#include "display.h"

using namespace std;

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


pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo){

    coordinates coords = pixInfo.coords;

    byte w = buff[0]; //img[0];
    byte sr = buff[1]; //img[1];
    int amountPixW = w;
    int amountPixB = (sr >> 3) & ((1 << 5)-1);
    int amountPixR = (((1 << 3) - 1) & (sr >> (6 - 1)));


    //aktuelle Baustelle!!
    coords = draw2Byte(amountPixW, amountPixB, amountPixR, coords);


  
    
    //int anzS = (sr >> 3) & ((1 << 5)-1);
    //int anzR = (((1 << 3) - 1) & (sr >> (6 - 1)));
/*
    // White
    coords = setPixels(amountPixW, palette[1], lastX, lastY);
    lastX = coords.x;
    lastY = coords.y;
    // Black
    coords = setPixels(amountPixB, palette[0], lastX, lastY);
    lastX = coords.x;
    lastY = coords.y;
    // Red
    coords = setPixels(amountPixR, palette[2], lastX, lastY);
    lastX = coords.x;
    lastY = coords.y;
    */

    
    pixInfo.totalAmountPixW += amountPixW;
    pixInfo.totalAmountPixB += amountPixB;
    pixInfo.totalAmountPixR += amountPixR;
    pixInfo.coords = coords;

    return pixInfo;
}

coordinates draw2Byte(int amountPixW, int amountPixB, int amountPixR, coordinates coords){
    
    /*
    // draw white pixels
    int filledLines = amountPixW / SCREEN_WIDTH;
    int pixelsLef = amountPixW % SCREEN_WIDTH;
    gfx.setColor(palette[1]);
    if(filledLines > 0)
        for(int i = 0; i<filledLines; i++){
            gfx.drawLine(coords.x, coords.y, SCREEN_WIDTH, coords.y);
            coords.y += 1;
        }
    if(pixelsLef > 0){
        gfx.drawLine(coords.x, coords.y, (coords.x + pixelsLef), coords.y);
        coords.x = pixelsLef;
    }

    // draw black pixels
    filledLines = amountPixB / SCREEN_WIDTH;
    pixelsLef = amountPixB % SCREEN_WIDTH;
    gfx.setColor(palette[0]);
    if(filledLines > 0)
        for(int i = 0; i<filledLines; i++){
            gfx.drawLine(coords.x, coords.y, SCREEN_WIDTH, coords.y);
            coords.y += 1;
        }
    if(pixelsLef > 0){
        gfx.drawLine(coords.x, coords.y, (coords.x + pixelsLef), coords.y);
        coords.x = pixelsLef;
    }

    // draw white pixels
    filledLines = amountPixR / SCREEN_WIDTH;
    pixelsLef = amountPixR % SCREEN_WIDTH;
    gfx.setColor(palette[2]);
    if(filledLines > 0)
        for(int i = 0; i<filledLines; i++){
            gfx.drawLine(coords.x, coords.y, SCREEN_WIDTH, coords.y);
            coords.y += 1;
        }
    if(pixelsLef > 0){
        gfx.drawLine(coords.x, coords.y, (coords.x + pixelsLef), coords.y);
        coords.x = pixelsLef;
    }
    */

 
    
    // draw white pixels
    for(int i = 0; i< amountPixW; i++){
            gfx.setColor(palette[1]);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(i==SCREEN_WIDTH){
                coords.y += 1;
                coords.x = 1;
            }
    }

    // draw black pixels
    for(int i = 0; i< amountPixB; i++){
            gfx.setColor(palette[0]);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(i==SCREEN_WIDTH){
                coords.y += 1;
                coords.x = 1;
            }
    }

    // draw red pixels
    for(int i = 0; i< amountPixR; i++){
            gfx.setColor(palette[2]);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(i==SCREEN_WIDTH){
                coords.y += 1;
                coords.x = 1;
            }
    }  

    return coords;
}


/*
coordinates setPixels(int anzPixel, int col, int lastX, int lastY){
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
    coords.x = lastX;
    coords.y = lastY;

    return coords;
}*/

void gfxInit(){
    Serial.println("Initializing display");
    gfx.init();
}

void gfxClearBuffer(){
    Serial.println("Clearing display buffer");
    gfx.fillBuffer(0);
}

void gfxCommitBuffer(){
    Serial.println("Committing display buffer");
    gfx.commit();
}

void gfxDemo(){
    Serial.println("Starting Display Demo");
    uint8_t rotation = 0;

    for(int i=0; i<3; i++){
        gfx.fillBuffer(0);
        gfx.setColor(1);
        gfx.drawLine(0, 0, 20, 20);
        gfx.setColor(13);
        gfx.fillCircle(20, 20, 5);
        gfx.commit();

        gfx.setRotation(rotation);
        gfx.fillBuffer(3);
        gfx.setColor(0);
        gfx.setFont(ArialMT_Plain_10);
        gfx.drawLine(0, 0, gfx.getWidth(), gfx.getHeight());
        gfx.drawString(10, 10, "Hello World");
        gfx.setFont(ArialMT_Plain_16);
        gfx.drawString(10, 30, "Something works! Something does not...");
        gfx.setFont(ArialMT_Plain_24);
        gfx.drawString(10, 55, "Yes!");

        gfx.commit();
        Serial.println("Commited buffer");
        rotation = (rotation + 1) % 4;

        i+=1;
    }
}

    	
 
