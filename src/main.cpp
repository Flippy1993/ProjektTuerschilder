#include <Arduino.h>

const int LED = 5; // I/O-Pin, Board LED // ak

void setup() {
    // put your setup code here, to run once:
    pinMode(LED, OUTPUT); // PIN 5 wird Ausgabe-PIN
}

void loop() {
    // put your main code here, to run repeatedly:
    digitalWrite(LED, LOW); // LED ausschalten
    delay(1000); // Eine Sekunde warten
    digitalWrite(LED, HIGH); // LED einschalten
    delay(1000); // Eine Sekunde warten
}

// AK: Testkommentar; PG: Scheint zu gehen