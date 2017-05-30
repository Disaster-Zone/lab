#include <Wire.h>
#include "Adafruit_VCNL4010.h"
#include "Adafruit_SI1145.h"

Adafruit_VCNL4010 vcnl;
Adafruit_SI1145   uv = Adafruit_SI1145();


#if defined(ARDUINO_ARCH_SAMD)
// for Zero, output on USB Serial console, remove line below if using programming port to program the Zero!
   #define Serial SerialUSB
#endif

void setup() {
  Serial.begin(9600);
  Serial.println("VCNL4010 test");

  if (! vcnl.begin()) {
    Serial.println("VCNL4010 not found");
    while (1);
  }
  Serial.println("Found VCNL4010");

  if (! uv.begin()) {
    Serial.println("Si1145 not found");
    while (1);
  }
  Serial.println("Found Si1145");
}


void loop() {
  Serial.println("================== VCNL ==================");
  Serial.print("Ambient  : "); Serial.println(vcnl.readAmbient());
  Serial.print("Proximity: "); Serial.println(vcnl.readProximity());

  Serial.println("================== UV ==================");
  Serial.print("Visable  : "); Serial.println(uv.readVisible());
  Serial.print("InfraRed : "); Serial.println(uv.readIR());

  // Uncomment if you have an IR LED attached to LED pin!
  // Serial.print("Prox: "); Serial.println(uv.readProx());

  float UVIndex = uv.readUV();
  // the index is multiplied by 100 so to get the
  // integer index, divide by 100!
  UVIndex /= 100.0;
  Serial.print("UV:      : "); Serial.println(UVIndex);
  Serial.println("");
  
  delay(1000);
}
