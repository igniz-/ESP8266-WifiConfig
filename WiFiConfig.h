#ifndef _WIFICONF      //tells compiler to compile this class only once
#define _WIFICONF

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>

struct WiFiData {
  byte connected;
  char ssid[32];
  char psk[64];
};

class WiFiConfig {
    private:
        const byte DNS_PORT = 53;
        String responseHTML;
        String rows;
        bool connected = false;
        DNSServer dnsServer;
        WiFiData data;

        void connect(WiFiData *data);
        void saveData(WiFiData *data);
    public:
        WiFiConfig();
        ~WiFiConfig();
        bool isConnected();
        void start();
        ESP8266WebServer *webServer;
};


#endif