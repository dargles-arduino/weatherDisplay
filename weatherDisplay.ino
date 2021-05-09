/**
 * Program: weatherDisplay
 * Purpose:
 *   The aim of this program is to contact weather.org for a weather forecast and to 
 * display it. Then I want to make it possible to contact my own weather server to 
 * display the latest info from that.
 * @author: David Argles, d.argles@gmx.com 
 */
#define PROGRAM "weatherDisplay"
#define VERSION "1.0"
#define BUILD   "09 May 2021 @13:45h"

// Library includes
#include <Arduino.h>
#include <ESP8266WiFi.h>
//#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>

// Includes for our own files
#include "flashscreen.h"
#include "myInfo.h"
#include "terminal.h"

// Instantiate the various classes
flashscreen       flash;
terminal          display;
//ESP8266WiFiMulti  WiFiMulti;

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  flash.message(PROGRAM, VERSION, BUILD);
  display.print("\nProgram:\n");
  display.print(PROGRAM);

  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  display.print("\nConnecting\n");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    Serial.flush();
    delay(1000);
  }
  String ipAddr = "IP:"+WiFi.localIP().toString();
  display.print(ipAddr);
}

void loop() {
  // wait for WiFi connection
  if ((WiFi.status() == WL_CONNECTED)) {

    WiFiClient client;
    HTTPClient http; //must be declared after WiFiClient for correct destruction order, because used by http.begin(client,...)

    Serial.print("[HTTP] begin...\n");

    // configure server and url
    http.begin(client, "http://jigsaw.w3.org/HTTP/connection.html");
    //http.begin(client, "jigsaw.w3.org", 80, "/HTTP/connection.html");

    Serial.print("[HTTP] GET...\n");
    // start connection and send HTTP header
    int httpCode = http.GET();
    if (httpCode > 0) {
      // HTTP header has been send and Server response header has been handled
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);

      // file found at server
      if (httpCode == HTTP_CODE_OK) {

        // get length of document (is -1 when Server sends no Content-Length header)
        int len = http.getSize();

        // create buffer for read
        uint8_t buff[128] = { 0 };

#if 0
        // with API
        Serial.println(http.getString());
#else
        // or "by hand"

        // get tcp stream
        WiFiClient * stream = &client;

        // read all data from server
        while (http.connected() && (len > 0 || len == -1)) {
          // read up to 128 byte
          int c = stream->readBytes(buff, std::min((size_t)len, sizeof(buff)));
          Serial.printf("readBytes: %d\n", c);
          if (!c) {
            Serial.println("read timeout");
          }

          // write it to Serial
          Serial.write(buff, c);

          if (len > 0) {
            len -= c;
          }
        }
#endif

        Serial.println();
        Serial.print("[HTTP] connection closed or file end.\n");
      }
    } else {
      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
    }

    http.end();
  }

  delay(60000);
}
