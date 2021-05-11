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
#define BUILD   "11 May 2021 @20:53h"

// Library includes
#include <Arduino.h>
#include <ESP8266WiFi.h>

// Includes for our own files
#include "flashscreen.h"
#include "myInfo.h"
#include "terminal.h"

// Instantiate the various classes
flashscreen       flash;
terminal          display;

// Start the webserver (note, this is before starting the WiFi!)
WiFiServer webserver(80);

void setup() {
  Serial.begin(115200);
  // Serial.setDebugOutput(true);
  flash.message(PROGRAM, VERSION, BUILD);
  display.print("\nProgram:\n");
  display.print(PROGRAM);

  // Configure the WiFi
  WiFi.hostname(WIFI_HOSTNAME);
  WiFi.mode(WIFI_STA);
  // Start up the WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  // Wait for the WiFi to get going
  display.print("\nStarting up");
  for(int i=4;i>0;i--){
    display.print(".");
    delay(1000);
  }
  // Check the WiFi is going, wait a bit longer if necessary
  display.print("\nConnecting");  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    Serial.flush();
    delay(1000);
  }
  webserver.begin();
  display.print("\nServer begun");
  String ipAddr = "\nIP:"+WiFi.localIP().toString();
  display.print(ipAddr);
}

void loop() {
  // Check if a client has connected
  WiFiClient client = webserver.available();
  yield();
  if (!client){
    return;
  }
  display.print("\nIncoming:\nb:");

  client.setTimeout(5000); // default is 1000

  // Read the first line of the request
  String req = client.readStringUntil('\r');
  // display.print(req);
  // Get battery value
  int start = req.indexOf("&bat=");
  int end = req.indexOf("&ser");
  //Serial.print(index);
  String val = req.substring(start+5,end);
  display.print(val);
  display.print("V\nt:");
  // Get temp
  start = req.indexOf("&temp=");
  end = req.indexOf("&pres");
  //Serial.print(index);
  val = req.substring(start+6,end);
  display.print(val);
  display.print("C\np:");
  // Get pressure
  start = req.indexOf("&pres=");
  end = req.indexOf("&hum");
  //Serial.print(index);
  val = req.substring(start+6,end);
  display.print(val);
  display.print("mB\nh:");
  // Get humidity
  start = req.indexOf("&hum=");
  end = req.indexOf("&err");
  //Serial.print(index);
  val = req.substring(start+5,end);
  display.print(val);
  display.print("%");

  // Read rest of request
  while (client.available()) client.read();

  // Respond to client
  client.print(F("HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<!DOCTYPE HTML>\r\n<html>\r\nTa!</html>"));
  // As we end, the server will automatically disconnect
  // display.print("\nDisconnecting");

}
