#include <Arduino.h>
#include "MiniGrafx.h"
#include <SPI.h>
#include "ILI9341_SPI.h" // Hardware-specific library
#include "EPD_WaveShare_75.h"
//#include "image.h"


#define CS 16
#define RST 4
#define DC 17
#define BUSY 27

#define BITS_PER_PIXEL 2

uint16_t palette[] = {0x0, 0x3, 0x4}; //black, white, red
//uint16_t palette[] = {0, 1, 15}; //black, white, yellow
/*uint16_t palette[] = {ILI9341_BLACK,     //  0
                      ILI9341_WHITE,     //  1
                      ILI9341_NAVY,      //  2
                      ILI9341_DARKCYAN,  //  3
                      ILI9341_DARKGREEN, //  4
                      ILI9341_MAROON,    //  5
                      ILI9341_PURPLE,    //  6
                      ILI9341_OLIVE,     //  7
                      ILI9341_LIGHTGREY, //  8
                      ILI9341_DARKGREY,  //  9
                      ILI9341_BLUE,      // 10
                      ILI9341_GREEN,     // 11
                      ILI9341_CYAN,      // 12
                      ILI9341_RED,       // 13
                      ILI9341_MAGENTA,   // 14
                      ILI9341_YELLOW};   // 15
*/

const int LED = 5; // I/O-Pin, Board LED // ak

EPD_WaveShare75 epd(CS, RST, DC, BUSY);
MiniGrafx gfx = MiniGrafx(&epd, BITS_PER_PIXEL, palette);

void setup() {
    // put your setup code here, to run once:   

    Serial.begin(115000); // ak: Datenuebertragungsrate auf 115000 Bits/s stellen. (VS Code Default)
    pinMode(LED, OUTPUT); // PIN 5 wird Ausgabe-PIN



    gfx.init();
    gfx.fillBuffer(0x4);
    
    gfx.commit();

    Serial.println("setup() done!");

}


void loop() {
    // put your main code here, to run repeatedly:
    /*gfx.fillBuffer(0);
    gfx.setColor(1);
    gfx.drawLine(0, 0, 20, 20);
    gfx.setColor(15);
    gfx.fillCircle(20, 20, 5);
    gfx.commit();
    */

    digitalWrite(LED, LOW); // LED ausschalten
    delay(1000); // Eine Sekunde warten
    digitalWrite(LED, HIGH); // LED einschalten
    delay(1000); // Eine Sekunde warten
    digitalWrite(LED, LOW);
    
    Serial.println("loop() done!");
}



