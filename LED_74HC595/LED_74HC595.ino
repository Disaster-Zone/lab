int LATCH_PIN = 5;
int CLOCK_PIN = 6;
int DATA_PIN  = 4;

byte leds = 0;

void setup() {
  pinMode(LATCH_PIN, OUTPUT);
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
}

void loop() {
  leds = 0;
  updateShiftRegister();
  delay(500);

  for (int i=0; i < 8; i++) {
    leds = 0;               // Zero out the LEDs each loop
    updateShiftRegister();  // to create a rolling LED
    
    bitSet(leds, i);
    updateShiftRegister();
    delay(500);
  }
}


void updateShiftRegister() {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, leds);
  digitalWrite(LATCH_PIN, HIGH);
}

