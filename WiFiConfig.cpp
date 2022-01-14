#include <EEPROM.h>
#include "WiFiConfig.h"
#include "config.h"

void WiFiConfig::start() {
    //check the eeprom data first
    if (WFC_EEPROM_INIT) {
        EEPROM.begin(sizeof(WiFiData));
    }
    EEPROM.get(WFC_EEPROM_ADDR, data);

    connected = false;
    if (data.connected != 1) {
        IPAddress apIP(172, 217, 28, 1);
        webServer = new ESP8266WebServer(80);

        rows = "";
        //This is the HTML for the captive portal
        //I tried to make it as minimal as possible
        responseHTML = "<!DOCTYPE html>"
                       "<meta charset=UTF-8>"
                       "<meta content=\"IE=edge\"http-equiv=X-UA-Compatible>"
                       "<meta content=\"width=device-width\"name=viewport>"
                       "<title>%page-title%</title>"
                       "<style>"
                       "#step1 p{text-align:center}"
                       "table{margin:auto}"
                       "td{padding:1em}"
                       "input,button{font-size: 1.5em}"
                       "#step1 table td:last-child::after{content:\"\\3009\"}"
                       "#step2{display:none}</style>"
                       "<div id=step1>"
                       "<p>%choose-network%</p>"
                       "<table>"
                       "<tr><th>SSID<th>Signal</tr>"
                       "%network-rows%"
                       "</table>"
                       "</div>"
                       "<div id=step2>"
                       "<nav>&#12296;<a href=# onclick=back()>Back</a></nav>"
                       "<div><table><tr><td>"
                       "<form action=/set method=POST>"
                       "<h2 id=networkName></h2>"
                       "<p>Password:</p><input name=pass type=password><input name=ssid type=hidden id=ssid>"
                       "<button>Connect</button>"
                       "</form>"
                       "</table></div></div>"
                       "<script>function NS(e){for(var t=e.target;\"TR\"!=t.nodeName;)t=t.parentNode;document.getElementById(\"networkName\").innerText=t.attributes[0].value,document.getElementById(\"ssid\").value=t.attributes[0].value,document.getElementById(\"step1\").style.display=\"none\",document.getElementById(\"step2\").style.display=\"block\"}function back(){document.getElementById(\"step1\").style.display=\"block\",document.getElementById(\"step2\").style.display=\"none\"}var tr=document.getElementsByTagName(\"tr\");for(i=0;i<tr.length;i++)tr[i].addEventListener(\"click\",NS)</script>";

        //scan for networks
        int numberOfNetworks = WiFi.scanNetworks();
        for (int i = 0; i < numberOfNetworks; i++) {
            rows += "<tr ssid=\"" + WiFi.SSID(i) + "\"><td>" + WiFi.SSID(i) + "</td><td><meter value=" + (2 * (WiFi.RSSI(i) + 100)) + " min=0 max=100></meter></td></tr>";
        }

        WiFi.mode(WIFI_AP);
        WiFi.softAPConfig(apIP, apIP, IPAddress(255, 255, 255, 0));
        WiFi.softAP(WFC_APSSID);

        // if DNSServer is started with "*" for domain name, it will reply with
        // provided IP to all DNS request
        dnsServer.start(DNS_PORT, "*", apIP);

        // replay to all requests with same HTML
        webServer->onNotFound([&]() {
            responseHTML.replace("%page-title%", WFC_C_PAGETITLE);
            responseHTML.replace("%choose-network%", WFC_C_CHOOSENETWORK);
            responseHTML.replace("%network-rows%", rows);
            webServer->send(200, "text/html", responseHTML);
        });
        webServer->on("/set", [&]() {
            if (webServer->method() != HTTP_POST)
            {
                webServer->send(405, "text/plain", "Method Not Allowed");
            }
            else
            {
                WiFiData newData;
                webServer->arg("ssid").toCharArray(newData.ssid, 32);
                webServer->arg("pass").toCharArray(newData.psk, 64);
                newData.connected = 1;

                String message = "Connecting...";
                webServer->send(200, "text/plain", message);
                //try to connect
                connect(&newData);
            }
        });

        webServer->begin();
    } else {
        connect(&data);
    }
}

WiFiConfig::WiFiConfig() {
}

WiFiConfig::~WiFiConfig() {
    delete webServer;
}

bool WiFiConfig::isConnected() {
    if (!connected) {
        dnsServer.processNextRequest();
        webServer->handleClient();
    }

    return connected;
}

void WiFiConfig::connect(WiFiData *_data) {
    data = *_data;
    WiFi.mode(WIFI_STA);
    WiFi.begin(data.ssid, data.psk);

    int timeout = WFC_WIFI_TIMEOUT * 1000; // seconds to millisecond
    while (timeout > 0 && WiFi.status() != WL_CONNECTED) {
        delay(500);
        timeout -= 500;
    }

    if (timeout <= 0) {
        //timeout reached, restart AP
        data.connected = 0;
        start();
    } else {
        connected = true;
        saveData(&data);
    }
}

void WiFiConfig::saveData(WiFiData *data) {
    EEPROM.put(WFC_EEPROM_ADDR, *data);
    EEPROM.commit();
}