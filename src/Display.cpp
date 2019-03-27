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

#define SCREEN_WIDTH 641
#define SCREEN_HEIGHT 384
#define BITS_PER_PIXEL 2

uint16_t palette[] = {0, 3, 4}; //black, white, red

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

int sizeInByte = 0;
int imgWidth = 0;
int imgHeight = 0;

int runLengthPixW_H = 0;
int runLengthPixB_H = 0;
int runLengthPixR_H = 0;

pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo){
    
    /*###########################
                Header
      ########################### */
    if(pixInfo.amountBytes == 0){
        accessTiming at = pixInfo.at;
        at.timeLastAccess = IntValOf2Byte(buff);
        Serial.println("timeLastAccess:");
        Serial.println(at.timeLastAccess);
        pixInfo.at = at;
    }
    if(pixInfo.amountBytes == 2){
        accessTiming at = pixInfo.at;
        at.timeNextAccess = IntValOf2Byte(buff);
        Serial.println("timeNextAccess:");
        Serial.println(at.timeNextAccess);
        pixInfo.at = at;
    }
    if(pixInfo.amountBytes == 4){
        sizeInByte = IntValOf2Byte(buff);
        Serial.println("SizeInByte:");
        Serial.println(sizeInByte);
    }
    if(pixInfo.amountBytes == 6){
        imgWidth = IntValOf2Byte(buff);
        Serial.println("imgWidth:");
        Serial.println(imgWidth);
    }
    if(pixInfo.amountBytes == 8){
        imgHeight = IntValOf2Byte(buff);
        Serial.println("imgHeight:");
        Serial.println(imgHeight);
    }
    if(pixInfo.amountBytes == 10){
        runLengthPixW_H = int(buff[0]);
        Serial.println("amountPixW_H:");
        Serial.println(runLengthPixW_H);
        runLengthPixB_H = int(buff[1]);
        Serial.println("amountPixB_H:");
        Serial.println(runLengthPixB_H);
    }
    if(pixInfo.amountBytes == 12){
        runLengthPixR_H = int(buff[0]);
        Serial.println("amountPixR_H:");
        Serial.println(runLengthPixR_H);
    }

    /*###########################
                Body
      ########################### */

    if(pixInfo.amountBytes >= 14){

        coordinates coords = pixInfo.coords;

        int amountPixW = int(buff[0]);
        int amountPixB = int(buff[1] >> runLengthPixB_H);
        uint8_t buff_r = buff[1];
        for(int i = 7; i>= 0 + runLengthPixR_H; i--){
            bitClear(buff_r,i);
        }
        int amountPixR = int(buff_r);

        coords = draw2Byte(amountPixW, amountPixB, amountPixR, coords);    

        pixInfo.totalAmountPixW += amountPixW;
        pixInfo.totalAmountPixB += amountPixB;
        pixInfo.totalAmountPixR += amountPixR;    
        pixInfo.coords = coords;
    }
    pixInfo.amountBytes += 2;

    return pixInfo;
}

coordinates draw2Byte(int amountPixW, int amountPixB, int amountPixR, coordinates coords){

    // draw white pixels
    for(int i = 0; i< amountPixW; i++){
            gfx.setColor(1);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x >= imgWidth){
                coords.y += 1;
                coords.x = 0;
            }
    }

    // draw black pixels
    for(int i = 0; i< amountPixB; i++){
            gfx.setColor(3);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x >= imgWidth){
                coords.y += 1;
                coords.x = 0;
             }
    }


    // draw red pixels
    for(int i = 0; i< amountPixR; i++){
            gfx.setColor(2);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x >= imgWidth){
                coords.y += 1;
                coords.x = 0;
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

int IntValOf2Byte(uint8_t* buff){
    return int(buff[0] << 8) + int(buff[1]);
}

void gfxDemo(){

    Serial.println("Starting Display Demo");

    for(int i=0; i<3; i++){

        Serial.println("Height:");
        Serial.println(gfx.getHeight());
        Serial.println("Width:");
        Serial.println(gfx.getWidth());

        gfx.setRotation(0);
        gfx.fillBuffer(3);
        gfx.setColor(2);
        gfx.setColor(1);
        gfx.fillCircle((gfx.getWidth()/2), (gfx.getHeight()/2), 20);
        gfx.setColor(2);
        gfx.setPixel(1,1);
        gfx.setPixel(1,2);
        gfx.setPixel(2,1);
        gfx.setPixel(2,2);


        gfx.setPixel(gfx.getWidth()-1,gfx.getHeight()-1);
        gfx.setPixel(gfx.getWidth(),gfx.getHeight()-1);
        gfx.setPixel(gfx.getWidth() -1,gfx.getHeight());
        gfx.setPixel(gfx.getWidth(), gfx.getHeight());

        for(int b = 0; b<250; b++){
            gfx.setPixel(b,(gfx.getHeight()/2));
        }

        gfx.commit();
    }

    delay(10000);
}


    	
 
