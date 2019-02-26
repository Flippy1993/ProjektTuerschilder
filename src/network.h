#ifndef NETWORK_H    // To make sure you don't declare the function more than once by including the header multiple times.
#define NETWORK_H

#include <Wifi.h>

bool setupNetwork(const char * networkName, const char * hostDomain, int hostPort);

bool connectToWiFi(const char * ssid);

String convertMac2String(byte arrMac[6]);

String getMacAddress();

bool logout(String url);

void extractLoginParams(String url, String mac);

bool loginToWifi(String url);

void apiRequest(String url, bool textmode, bool demoMode);

void printLine();

#endif