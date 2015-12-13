#include <SPI.h>

const int dataReadyPin = 6;
const int chipSelectPin = 7;

void setup() {
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, LOW);
}

void loop() {
  for (int i=10; i<200; i+=10) {
    SPI.transfer(1);
    delay(i);
    SPI.transfer(0);
    delay(i);
  }
}
