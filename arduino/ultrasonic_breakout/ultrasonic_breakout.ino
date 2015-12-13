#include <SPI.h>

const int chipSelectPin = 10;

void setup() {
  Serial.begin(9600);
  SPI.beginTransaction(SPISettings(14000000, MSBFIRST, SPI_MODE0));
  SPI.begin();
  pinMode(chipSelectPin, OUTPUT);
  digitalWrite(chipSelectPin, HIGH);
}

void send(unsigned int n) {
  //Serial.print("Sending ");
  //Serial.println(n);
  digitalWrite(chipSelectPin, LOW);
  unsigned int response = SPI.transfer(n);
  digitalWrite(chipSelectPin, HIGH);
  //Serial.print("Received ");
  //Serial.println(response);
  //delay(10);
}

void loop() {
  while (1) {
    for (int i=0; i<255; i++) {
      send(i);
    }
  }
}
