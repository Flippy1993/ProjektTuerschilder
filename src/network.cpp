#include "network.h"
#include <iostream>
#include <string>
#include <numeric>
#include <iterator>
#include <cstdint>
#include <sstream>
#include <WiFi.h>
#include <HTTPClient.h>
#include <algorithm>
using namespace std;

struct login_t {
  String dst;
  String popup;
  String username;
  String password;
} login;


const int BUTTON_PIN = 0;
const int LED_PIN = 5;
String localIp;

boolean setupNetwork(const char * networkName, const char * networkPswd, const char * hostDomain, int hostPort)
{
  String sMacAddress;
  Serial.println("Resolving local MAC...");
  WiFi.begin(networkName, networkPswd);
  sMacAddress = getMacAddress();

  String url = "http://" + (String)hostDomain + "/login";
  
  // Connect to the WiFi network (see function below loop)
  Serial.println("Starting Connection to: " + String(networkName));
  connectToWiFi(networkName, networkPswd);

  //delay(3000);
  getLoginPage(url, sMacAddress);
  requestURL2(url);

  apiRequest();
 
  /* KA-WLAN Anmeldeseite requesten und parsen um Zugangstoken zu erhalten*/
  //Serial.println("Requesting login page to parse access token");
  //requestURL(hostDomain, hostPort, sMacAddress); 

  return true;
}

void connectToWiFi(const char * ssid, const char * pwd){
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
  localIp = WiFi.localIP().toString();
  Serial.println(WiFi.localIP());
}

void getLoginPage(String url, String mac){

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  Serial.println("Sending GET -> "+url+" -> Code:" + httpCode);
  if (httpCode > 0) { //Check for the returning code

      Serial.println("Extracting Request Params from Payload");
      String payload = http.getString();
      //Serial.println("HTTP Code: "+httpCode);
      //Serial.println("Response: "+payload);


      String raw = payload.substring(payload.indexOf("<form name=\"login\" action=\"http://cp.ka-wlan.de/login\" method=\"post\">"));
      raw = raw.substring(0,raw.indexOf("</td>"));
      String param_dst = raw.substring(raw.indexOf("name=\"dst\" value=\"")+18);
      param_dst = param_dst.substring(0,param_dst.indexOf("\" />"));
      String param_popup = raw.substring(raw.indexOf("name=\"popup\" value=\"")+20);
      param_popup = param_popup.substring(0,param_popup.indexOf("\""));
      String param_password = raw.substring(raw.indexOf("name=\"password\" type=\"password\" value=\"")+39);
      param_password = param_password.substring(0,param_password.indexOf("\""));

      login.dst = param_dst;
      login.popup = param_popup;      
      login.username = mac;
      login.password = param_password;

      Serial.println("dst-> "+login.dst);
      Serial.println("popup-> "+login.popup);
      Serial.println("username-> "+login.username);
      Serial.println("password-> "+login.password);
    }

  else {
    Serial.println("Error on HTTP request");
  }

  http.end(); //Free the resources
}

void requestURL2(String url){

  //String url = "http://cp.ka-wlan.de/login";

    HTTPClient http;
    //http.begin(url); //Specify the URL
    http.begin("http://cp.ka-wlan.de/login");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    /*http.addHeader("dst", login.dst);
    http.addHeader("popup", login.popup);
    http.addHeader("username", login.username);
    http.addHeader("password", login.password);*/

    //int httpCode = http.POST("?dst="+login.dst+"?popup="+login.popup+"?username="+login.username+"?password="+login.password); //Make the request

    while(login.username.indexOf(":") != -1){
      login.username.replace(":","%");
    }

    Serial.println(login.username);

    int httpCode = http.POST("dst=&popup=false&username="+login.username+"&password=w5wb8w096");

    Serial.println("Sending POST -> "+url+" -> Code:" + httpCode);

    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println("HTTP Code: "+httpCode);
        Serial.println("Response: "+payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
}

void apiRequest(){
    String url = "http://www.iwi.hs-karlsruhe.de/Intranetaccess/REST/buildings/facultyrooms/display/texts/ZZ-22-33-44-55-66";

    Serial.println("Sending request to: "+url);
    HTTPClient http;
    http.begin(url); //Specify the URL
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET(); //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println("HTTP Code: "+httpCode);
        Serial.println("Response: "+payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
}

void requestURL(const char * host, uint8_t port, String mac)
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

  // This will send the request to the server
  client.print((String)"GET / HTTP/1.1\r\n" +
               "Host: " + String(host) + "\r\n" +
               "User-Agent: Mozilla/5.0 () Gecko/20100101 Firefox/64.0\r\n" +
               "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n" +
               "Accept-Language: en-us,en;q=0.5\r\n" +  //--compressed
               "Referer: http://"+host+"/login\r\n" +
               "Connection: keep-alive\r\n" +
               "Upgrade-Insecure-Requests: 1\r\n" +
               "date dst=&popup=false&username="+mac+"&password=w5wb8w096\r\n"); //--date dst=&popup=false&username=[Mac-Adresse]&password=w5wb8w096   Frage:Passwort aus Mac generiert?
  unsigned long timeout = millis();
  while (client.available() == 0) 
  {
    if (millis() - timeout > 10000) 
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

void requestURL(const char * host, uint8_t port)
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