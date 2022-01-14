/****************************************************************
 * Change these settings to customise your application
 ****************************************************************/

// SSID for the AP when there is no WiFi info or cannot connect
#define WFC_APSSID "ESP8266"

// Title of the captive portal
#define WFC_C_PAGETITLE "ESP8266"
// Text at the top of the captive portal
#define WFC_C_CHOOSENETWORK "Choose Network"

// Set to false if you want to initialize the EEPROM yourself
// Be aware that we need sizeof(WiFiData) bytes to store the SSID and PSK
#define WFC_EEPROM_INIT true
// Address of the eeprom to store the wifi data
// There is usually no need to change unless you're storing data
// on the eeprom at this address. The size of the storef data is
// sizeof(WiFiData) => ~100 bytes
#define WFC_EEPROM_ADDR 0x0000
// Change this if you use the EEPROM
#define WFC_EEPROM_SPACE_EXTRA 0

// Timeout to connect to the WiFi in seconds
// If Timeout is reached then we go back to AP mode
#define WFC_WIFI_TIMEOUT 30
