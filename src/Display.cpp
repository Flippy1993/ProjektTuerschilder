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
#define BITS_PER_PIXEL 2

uint16_t palette[] = {0, 3, 4}; //black, white, red
//uint16_t palette[] = {0x00, 0x33, 0x44};

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);


pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo){

    coordinates coords = pixInfo.coords;

    byte w = buff[0]; //img[0];
    byte br = buff[1]; //img[1];
    //int amountPixW = w;
    int amountPixW = buff[0];
    //int amountPixB = (br >> 3) & ((1 << 5)-1);
    //int amountPixR = (((1 << 3) - 1) & (br >> (6 - 1)));
    int amountPixB = buff[1] >> 3;
    uint8_t b = buff[1] << 5;
    int amountPixR = b >> 5;

    /*Serial.println("amountPixW");
    Serial.println(amountPixW);
    Serial.println("amountPixB");
    Serial.println(amountPixB);
    Serial.println("amountPixR");
    Serial.println(amountPixR);*/


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
    
  
    // draw white pixels
    for(int i = 0; i< amountPixW; i++){
            gfx.setColor(1);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x > gfx.getWidth()){
                coords.y += 1;
                coords.x = 1;
            }
    }

    // draw black pixels
    for(int i = 0; i< amountPixB; i++){
            gfx.setColor(3);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x > gfx.getWidth()){
                coords.y += 1;
                coords.x = 1;
             }
    }

    // draw red pixels
    for(int i = 0; i< amountPixR; i++){
            gfx.setColor(2);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x > gfx.getWidth()){
                coords.y += 1;
                coords.x = 1;
            }
    }  

    return coords;
}

void gfxInit(){
    Serial.println("Initializing display");
    gfx.init();
}

void gfxClearBuffer(){
    Serial.println("Clearing display buffer");
    gfx.fillBuffer(3);
}

void gfxCommitBuffer(){
    Serial.println("Committing display buffer");
    gfx.commit();
}

void gfxSetRotation(int rot){
    Serial.println("Setting Display Rotation to:");
    Serial.println(rot);
    gfx.setRotation(rot);
}

void gfxDemo(){

    Serial.println("Starting Display Demo");

    for(int i=0; i<3; i++){

        gfx.setRotation(0);
        gfx.fillBuffer(3);
        gfx.setColor(2);
        gfx.drawLine(0, 0, gfx.getWidth(), gfx.getHeight());
        gfx.setColor(1);
        gfx.fillCircle((gfx.getWidth()/2), (gfx.getHeight()/2), 20);

        for(int b = 0; b<250; b++){
            gfx.setPixel(b,(gfx.getHeight()/2));
        }

        gfx.commit();
    }
}

    	
 
