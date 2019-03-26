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

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

int sizeInByte = 0;
int imgWidth = 0;
int imgHeight = 0;

bool printOnce1 = true;
bool printOnce2 = true;
bool printOnce3 = true;


pixelInfo refreshScreen(uint8_t* buff, pixelInfo pixInfo){
    pixInfo.amountBytes += 2;


    if(pixInfo.amountBytes == 6){

        for(int i = 0; i<16; i++){
            if(i<8){
            bitWrite(sizeInByte,i ,bitRead(buff[0],i));     
            }else{
            bitWrite(sizeInByte,i ,bitRead(buff[1],i-8)); 
            }
        }
    }
    if(pixInfo.amountBytes == 8){
        for(int i = 0; i<16; i++){
            if(i<8){
            bitWrite(imgWidth,i ,bitRead(buff[0],i));     
            }else{
            bitWrite(imgWidth,i ,bitRead(buff[1],i-8)); 
            }
        }
    }
    if(pixInfo.amountBytes == 10){
        for(int i = 0; i<16; i++){
            if(i<8){
            bitWrite(imgHeight,i ,bitRead(buff[0],i));     
            }else{
            bitWrite(imgHeight,i ,bitRead(buff[1],i-8)); 
            }
        }
    }

    if(sizeInByte > 0 && printOnce1){
        Serial.println("SizeInByte:");
        Serial.println(sizeInByte);
        printOnce1 = false;
    }
    if(imgWidth > 0 && printOnce2){
        Serial.println("imgWidth:");
        Serial.println(imgWidth);
        printOnce2 = false;
    }
    if(imgHeight > 0 && printOnce3){
        Serial.println("imgHeight:");
        Serial.println(imgHeight);
        printOnce3 = false;
    }
    


    if(pixInfo.amountBytes > 14){
        coordinates coords = pixInfo.coords;

        /*int amountPixW = buff[0];// << 1; //??
        uint8_t buff_1_b = buff[1];
        uint8_t buff_1_r = buff[1];
        int amountPixB = (int)(buff_1_b >> 3);
        uint8_t r = (buff_1_r << 5);
        int amountPixR = (int)(r >> 5);*/

        int amountPixW = 0;
        int amountPixB = 0;
        int amountPixR = 0;

        for(int i = 0; i<8; i++){
                bitWrite(amountPixW,i ,bitRead(buff[0],i));
            if(i < 3){
                bitWrite(amountPixR,i ,bitRead(buff[1],i));
            }else{
                bitWrite(amountPixB,i ,bitRead(buff[1],i+3));
            }           
            
        }

        coords = draw2Byte(amountPixW, amountPixB, amountPixR, coords);    

        pixInfo.totalAmountPixW += amountPixW;
        pixInfo.totalAmountPixB += amountPixB;
        pixInfo.totalAmountPixR += amountPixR;    
        pixInfo.coords = coords;
    }

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
                coords.x = 0;
            }
    }

    // draw black pixels
    for(int i = 0; i< amountPixB; i++){
            gfx.setColor(3);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x > gfx.getWidth()){
                coords.y += 1;
                coords.x = 0;
             }
    }


    // draw red pixels
    for(int i = 0; i< amountPixR; i++){
            gfx.setColor(2);
            gfx.setPixel(coords.x,coords.y);
            coords.x += 1;
            if(coords.x > gfx.getWidth()){
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
        //gfx.drawLine(0, 0, gfx.getWidth(), gfx.getHeight());
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


    	
 
