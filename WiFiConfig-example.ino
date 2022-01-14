#include "WiFiConfig.h"

WiFiConfig wifi;

bool led = false;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);

  //This initializes the WiFiConfig object
  wifi.start();
}

void loop() {
  //This call (wifi.isConnected) is required to make sure
  //WiFiConfig can handle the webserver and dnsserver
  //If it returns true it means you're connected to the configured network
  if (wifi.isConnected()) {
    //Your amazing code here
  } else {
    //if you're not connected the device will set itself in AP mode and will
    //wait to be configured
    led = !led;
    digitalWrite(LED_BUILTIN, led ? LOW : HIGH);

    delay(50);
  }
}