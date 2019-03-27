#include <Arduino.h>
#include "MiniGrafx.h"
#include <SPI.h>
#include "EPD_WaveShare_75.h"
#include "DisplayDriver.h"
#include "network.h"
#include <WiFi.h>
#include "display.h"
#include "sleep.h"


const int LED = 5; // I/O-Pin, Board LED // ak

const char * networkName = "KA-WLAN";
//const char * baseUrl = "cp.ka-wlan.de"; // url geändert 27.03.19
const char * baseUrl = "cph.ka-wlan.de";
const int port = 80;
const String apiUrlImage = "http://www.iwi.hs-karlsruhe.de/Intranetaccess/REST/buildings/facultyrooms/display/image/";
const String apiUrlText = "http://www.iwi.hs-karlsruhe.de/Intranetaccess/REST/buildings/facultyrooms/display/texts/";

void setup() {


    pinMode(LED, OUTPUT); // PIN 5 wird Ausgabe-PIN
    delay(500);
    digitalWrite(LED,HIGH);
    digitalWrite(LED, HIGH); // LED ausschalten
    delay(500);
    digitalWrite(LED, LOW); // LED einschalten
    delay(500);
    digitalWrite(LED, HIGH);


    Serial.begin(115200); 
    //Serial.begin(115000); // ak: Datenuebertragungsrate auf 115000 Bits/s stellen. (VS Code Default)

    Serial.println("Setting up Network Connection");
    bool connectionSuccesfull = setupNetwork(networkName,             // Netzwerkname
                                             baseUrl,                 // Basis Url KA-Wlan Login
                                             port);                   // Port                                                              
    String str = "failed!";
    if(connectionSuccesfull)
        str = "successful";
    Serial.println("Network Connection: " + str);

    Serial.println("setup() done!");

    if (connectionSuccesfull){
        // 1. param -> Textmode, 2. param -> Demo Mode
        apiRequest(apiUrlText, true, true); 
        delay(2);
        apiRequest(apiUrlImage, false, true);
    }

    //logout(baseUrl);

    // next access time
    int remainingSleepTime = getTimeNextAccess() - getTimeLastAccess();
    Serial.println("##################");
    Serial.print("last access was at: ");
    Serial.println(getTimeLastAccess());
    Serial.print("next access will be in: ");
    Serial.println(getTimeNextAccess());
    Serial.print("going to deep sleep for:");
    Serial.print(remainingSleepTime);
    Serial.println(" minutes!");
    Serial.println("##################");

    digitalWrite(LED, LOW); //LED off
    goToDeepSleep(remainingSleepTime, 2);
}

void loop() {

    delay(5000);
    
    Serial.println("loop() done!"); 
}




