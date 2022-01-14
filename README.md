# ESP8266-WifiConfig
A simple way to manage wifi connection of ESP8266 using a captive portal

## How to use
1. Create a new project.
2. Copy the files to the same folder as the project (WiFiConfig.h, WiFiConfig.cpp and config.h).
3. Edit the config.h file to change to your preference.
4. Init the WiFiConfig object
5. Call the start method on start
6. Call the isConnected method on the loop

## Why is this not a library?
I found it less messy to have a config.h file that you can edit rather than parameters to pass to the start function, but hey, here's the code, just change it as you please.
