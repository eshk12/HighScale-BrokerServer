#include <Arduino.h>
//#include <WiFi.h>
#include <WebSocketsClient.h>
#include "StompClient.h"
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <WebServer_WT32_ETH01.h>

#define MYPORT_TX 14
#define MYPORT_RX 15
#define DEBUG_ETHERNET_WEBSERVER_PORT       Serial

// Debug Level from 0 to 4
#define _ETHERNET_WEBSERVER_LOGLEVEL_       0


/**
* WiFi settings
**/

const byte GREENLED = 12;      // IO12 == 16 // POWER LED
const byte YELLOWLED = 4;       // IO4 == 18 // SCALE LED
const byte BLUELED = 2;      // IO2 == 19 // ETHERNET LED

const char* wlan_ssid             = "itzik home";
const char* wlan_password         = "a2b1c4d3";


/**
* Ethernet settings
*/

// Select the IP address according to your local network
IPAddress myIP(192, 168, 1, 232);
IPAddress myGW(192, 168, 1, 1);
IPAddress mySN(255, 255, 255, 0);

// Google DNS Server IP
IPAddress myDNS(8, 8, 8, 8);


/**
* Stomp server settings
**/
bool useWSS                       = false;
const char* ws_host               = "192.168.1.50";
const int ws_port                 = 8080;
const char* ws_baseurl            = "/HighScaleStompServer/"; // don't forget leading and trailing "/" !!!
char* deviceId = "A1B2C3D4";
char* subscribeDestination = "/stompserver/subscriber/";
char* sendDestination = "/stompserver/api/v1/sendmessage/";


// VARIABLES
String inString = "";
float weight;
WebSocketsClient webSocket;
SoftwareSerial myPort;
Stomp::StompClient stomper(webSocket, ws_host, ws_port, ws_baseurl, true);
boolean getScaleSample = false;

// Once the Stomp connection has been made, subscribe to a topic
void subscribe(Stomp::StompCommand cmd) {
  Serial.println("Connected to STOMP broker");
  stomper.subscribe(concatinateString(subscribeDestination, deviceId), Stomp::CLIENT, handleResponse);

  sendMessage(concatinateString(deviceId," is logged!"));
}

void error(const Stomp::StompCommand cmd) {
  Serial.println("ERROR: " + cmd.body);
}

Stomp::Stomp_Ack_t handleResponse(const Stomp::StompCommand cmd) {
  Serial.println(cmd.body);
  if(cmd.body == "/getScale"){
    getScaleSample = true;
    sendMessage("getScale");
  }else if(cmd.body == "/getScaleState"){
    sendMessage("I'm alive");
  }else if(cmd.body == "/getIp"){
    sendMessage((String)ETH.localIP());
  }else if(cmd.body == "/turnOnCommunicationLed"){
    sendMessage("turnOnCommunicationLed");
  }else if(cmd.body == "/getLocation"){
    sendMessage("getLocation");
  }

  return Stomp::CONTINUE;
}

void sendMessage(String message) {
  stomper.sendMessage(concatinateString(sendDestination, deviceId), message);
}

char* concatinateString(char* str1, char* str2) {
  char *buffer = (char*)malloc(strlen(str1) + strlen(str2)+1);
  sprintf(buffer, "%s%s", str1, str2);

  return buffer;
}

void turnLedOn(byte ledNo){
  if(digitalRead(ledNo != HIGH)){
      digitalWrite(ledNo, HIGH);
  }
}

void turnLedOff(byte ledNo){
  if(digitalRead(ledNo != LOW)){
      digitalWrite(ledNo, LOW);
    }
}

void setup() {
  pinMode(BLUELED, OUTPUT);
  pinMode(GREENLED, OUTPUT);
  pinMode(YELLOWLED, OUTPUT);

  turnLedOn(GREENLED);

  Serial.begin(115200);

  Serial.print("\nStarting BasicHttpClient on " + String(ARDUINO_BOARD));
  Serial.println(" with " + String(SHIELD_TYPE));
  Serial.println(WEBSERVER_WT32_ETH01_VERSION);

  // To be called before ETH.begin()
  WT32_ETH01_onEvent();
  ETH.begin(ETH_PHY_ADDR, ETH_PHY_POWER);
  ETH.config(myIP, myGW, mySN, myDNS);

  WT32_ETH01_waitForConnect();
  Serial.print(F("HTTP EthernetWebServer is @ IP : "));
  Serial.println(ETH.localIP());


  myPort.begin(9600, SWSERIAL_8N1, MYPORT_RX, MYPORT_TX, false);
  if (!myPort) { // If the object did not initialize, then its configuration is invalid
    Serial.println("Invalid SoftwareSerial pin configuration, check config");
    while (1) { // Don't continue with invalid configuration
      delay (1000);
    }
  }

  // Start the StompClient
  stomper.onConnect(subscribe);
  stomper.onError(error);

  if (useWSS) {
    stomper.beginSSL();
  } else {
    stomper.begin();
  }
}



void loop() {
  webSocket.loop();
  if (WT32_ETH01_isConnected()){
    turnLedOn(BLUELED);
  }else{
    turnLedOff(BLUELED);
  }

  if(myPort.available() > 0){
    turnLedOn(YELLOWLED);
    int inChar = myPort.read();
    if(inChar != '\n'){
      inString += (char)inChar;
    }else{
      inString.remove(0,2);
      weight = inString.toFloat();
      if(getScaleSample){
        sendMessage((String) weight);
        getScaleSample = false;
      }
      inString = "";
    }
  }else{
    turnLedOff(YELLOWLED);
  }

}
