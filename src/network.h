#ifndef NETWORK_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define NETWORK_H

#include <Wifi.h>

boolean setupNetwork(const char * networkName, const char * networkPswd, const char * hostDomain, int hostPort);

void connectToWiFi(const char * ssid, const char * pwd);

String convertMac2String(byte arrMac[6]);

String getMacAddress();

void getLoginPage(String url, String mac);

void requestURL(const char * host, uint8_t port);

void requestURL(const char * host, uint8_t port, String sMacAddress);

void requestURL2(String url);

void apiRequest();

void printLine();

#endif