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
boolean connected = false;
boolean loggedIn = false;

bool setupNetwork(const char * networkName, const char * hostDomain, int hostPort)
{
  bool successfull = false;
  String sMacAddress;
  Serial.println("Resolving local MAC...");
  WiFi.begin(networkName,"");
  sMacAddress = getMacAddress();  
  
  // Connect to the WiFi network (see function below loop)
  Serial.println("Starting Connection to: " + String(networkName));
  successfull = connectToWiFi(networkName);

  String url = "http://" + (String)hostDomain + "/login";
  extractLoginParams(url, sMacAddress);
  successfull = loginToWifi(url);  
 
  return successfull;
}

bool connectToWiFi(const char * ssid){
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
  if(WiFi.localIP().toString() != "")
    connected = true;
  else
    connected = false;

  return connected;  
}

void extractLoginParams(String url, String mac){

  HTTPClient http;
  http.begin(url);
  int httpCode = http.GET();
  Serial.println("Sending GET -> "+url+" -> Code:" + httpCode);
  if (httpCode > 0) { //Check for the returning code

      Serial.println("Extracting Request Params from Payload");
      String payload = http.getString();
 
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

bool loginToWifi(String url){

    HTTPClient http;
    http.begin(url);
    http.begin("http://cp.ka-wlan.de/login");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Replacing all : in mac address with % for formatting as url parameter
    while(login.username.indexOf(":") != -1){
      login.username.replace(":","%");
    }

    int httpCode = http.POST("dst="+login.dst+"&popup="+login.popup+"&username="+login.username+"&password="+login.password);

    Serial.println("Sending POST -> "+url+" -> Code:" + httpCode);

    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        Serial.println("HTTP Code: "+httpCode);
        //Serial.println("Response: "+payload);
        loggedIn = true;
      }
 
    else {
      Serial.println("Error on HTTP request");
      loggedIn = false;
    }
 
    http.end(); //Free the resources
    return loggedIn;
}

void apiRequest(String url, boolean demoMode){
    //String url = "http://www.iwi.hs-karlsruhe.de/Intranetaccess/REST/buildings/facultyrooms/display/texts/ZZ-22-33-44-55-66";

    String demoMac = "ZZ-22-33-44-55-66";

    while(login.username.indexOf(":") != -1){
      login.username.replace("%","-");
    }

    if(demoMode)
      url += demoMac;
    else
      url += login.username;

    Serial.println("Sending request to: "+url);
    HTTPClient http;
    http.begin(url); //Specify the URL
    http.addHeader("Content-Type", "text/plain");
    int httpCode = http.GET(); //Make the request
 
    if (httpCode > 0) { //Check for the returning code
 
        String payload = http.getString();
        //Stream s = http.getStream();
        Serial.println("HTTP Code: "+httpCode);
        Serial.println("Response: "+payload);
      }
 
    else {
      Serial.println("Error on HTTP request");
    }
 
    http.end(); //Free the resources
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