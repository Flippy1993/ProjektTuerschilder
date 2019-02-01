#include "network.h"
#include <iostream>
#include <string>
#include <numeric>
#include <iterator>
#include <cstdint>
#include <sstream>
#include <WiFi.h>

const int BUTTON_PIN = 0;
const int LED_PIN = 5;

boolean setupNetwork(const char * networkName, const char * networkPswd, const char * hostDomain, int hostPort)
{
  String sMacAddress;

  // Connect to the WiFi network (see function below loop)
  Serial.println("Starting Connection to: " + String(networkName));
  connectToWiFi(networkName, networkPswd);

  Serial.println("Resolving local MAC...");
  WiFi.begin(networkName, networkPswd);
  sMacAddress = getMacAddress();

  delay(3000);

  /* KA-WLAN Anmeldeseite requesten und parsen um Zugangstoken zu erhalten*/
  Serial.println("Requesting login page to parse access token");
  requestURL(hostDomain, hostPort, sMacAddress); // Connect to server

  return true;
}

void connectToWiFi(const char * ssid, const char * pwd)
{
  int ledState = 0;

  printLine();
  Serial.println("Connecting to WiFi network: " + String(ssid));

  while (WiFi.status() != WL_CONNECTED) 
  {
    // Blink LED while we're connecting:
    digitalWrite(LED_PIN, ledState);
    ledState = (ledState + 1) % 2; // Flip ledState
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected!");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}


void requestURL(const char * host, uint8_t port, String)
{
  printLine();
  Serial.println("Connecting to domain: " + String(host));

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  if (!client.connect(host, port))
  {
    Serial.println("connection failed");
    return;
  }
  Serial.println("Connected!");
  printLine();

/*
  //Get Mac-Adress
  uint8_t l_Mac[6];
  //esp_wifi_get_mac(ESP_IF_WIFI_STA, l_Mac);
  WiFi.macAddress(l_Mac);
  std::string s;
    s.reserve( 6 );

    for ( int value : l_Mac ) s += std::to_string( value ) + ' ';

    std::cout << s << std::endl;
  //Serial.println("Mac-Address: " + l_Mac);

  //Mac-Adresse rausfinden -> in String kompilieren -> in der Anfrage mitschicken
*/


  // This will send the request to the server
  client.print((String)"GET / HTTP/1.1\r\n" +
               "Host: " + String(host) + "\r\n" +
               "User-Agent: Mozilla/5.0 () Gecko/20100101 Firefox/64.0\r\n" +
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n" +
               "Accept-Language: en-us,en;q=0.5\r\n" +  //--compressed
               "Referer: http://cp.ka-wlan.de/login\r\n" +
               "Connection: keep-alive\r\n" +
               "Upgrade-Insecure-Requests: 1\r\n" +
               "\r\n"); //--date dst=&popup=false&username=[Mac-Adresse]&password=w5wb8w096   Frage:Passwort aus Mac generiert?
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 5000) 
    {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) 
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

  Serial.println();
  Serial.println("closing connection");
  client.stop();
}

String getMacAddress()
{
  String sMacAddress;
  byte mac[6];

  WiFi.macAddress(mac);

  sMacAddress = convertMac2String(mac);
  
  Serial.println("MAC: "+sMacAddress);
  return sMacAddress;
}

String convertMac2String(byte arrMac[6]){
  String sMac;
  for (byte i = 0; i < 6; ++i)
  {
    char buf[3];
    sprintf(buf, "%2X", arrMac[i]);
    sMac += buf;
    if (i < 5) sMac += ':';
  }
  return sMac;
}


void printLine()
{
  Serial.println();
  for (int i=0; i<30; i++)
    Serial.print("-");
  Serial.println();
}