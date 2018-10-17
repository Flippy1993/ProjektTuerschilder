#include <Arduino.h>
#include "MiniGrafx.h"
#include <SPI.h>
#include "EPD_WaveShare_75.h"
#include "DisplayDriver.h"


#define CS 16
#define RST 4
#define DC 17
#define BUSY 27

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 384
#define BITS_PER_PIXEL 1

uint16_t palette[] = {0, 3, 4}; //black, white, red
//uint16_t palette[] = {0x00, 0x33, 0x44};

const int LED = 5; // I/O-Pin, Board LED // ak

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);


void setup() {
    // put your setup code here, to run once:   

    Serial.begin(115200); // ak: Datenuebertragungsrate auf 115000 Bits/s stellen. (VS Code Default)
    pinMode(LED, OUTPUT); // PIN 5 wird Ausgabe-PIN



    gfx.init();
    gfx.setRotation(1);
    /*gfx.fillBuffer(2);
    
    gfx.commit();*/

    Serial.println("setup() done!");

}

uint8_t rotation = 0;

void loop() {
    // put your main code here, to run repeatedly:
    /*gfx.fillBuffer(0);
    gfx.setColor(1);
    gfx.drawLine(0, 0, 20, 20);
    gfx.setColor(13);
    gfx.fillCircle(20, 20, 5);
    gfx.commit();

    digitalWrite(LED, LOW); // LED ausschalten
    delay(1000); // Eine Sekunde warten
    digitalWrite(LED, HIGH); // LED einschalten
    delay(1000); // Eine Sekunde warten
    digitalWrite(LED, LOW);*/

    gfx.setRotation(rotation);
    gfx.fillBuffer(3);
    gfx.setColor(0);
    gfx.setFont(ArialMT_Plain_10);
    gfx.drawLine(0, 0, gfx.getWidth(), gfx.getHeight());
    gfx.drawString(10, 10, "Hello World");
    gfx.setFont(ArialMT_Plain_16);
    gfx.drawString(10, 30, "Everything works!");
    gfx.setFont(ArialMT_Plain_24);
    gfx.drawString(10, 55, "Yes!");

    gfx.commit();
    Serial.println("Commited buffer");
    rotation = (rotation + 1) % 4;
    delay(5000);
    
    Serial.println("loop() done!");
}



