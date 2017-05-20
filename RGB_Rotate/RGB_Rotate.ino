#include <Bridge.h>
#include <BridgeClient.h>
#include <BridgeServer.h>
#include <BridgeSSLClient.h>
#include <BridgeUdp.h>
#include <Console.h>
#include <FileIO.h>
#include <HttpClient.h>
#include <Mailbox.h>
#include <Process.h>
#include <YunClient.h>
#include <YunServer.h>

#define RED_PIN 3
#define GREEN_PIN 5
#define BLUE_PIN 6

#define COMMON_ANODE

void setup() {
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
}

void loop() {
  setColor(255,   0,   0);  // red
  delay(1000);
  setColor(  0, 255,   0);  // green
  delay(1000);
  setColor(  0,   0, 255);  // blue
  delay(1000);
  setColor(255, 255,   0);  // yellow
  delay(1000);
  setColor( 80,   0,  80);  // purple
  delay(1000);
  setColor(  0, 255, 255);  // aqua
  delay(1000);  
}

void setColor(int red, int green, int blue) {
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

