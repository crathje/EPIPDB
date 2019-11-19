#include <Arduino.h>
#include <HardwareSerial.h>

#include <EPIPDB.h>
EPIPDB EPIPDB;

#define EP_RXD 25
#define EP_TXD 14
HardwareSerial EPSerial(1);

void setup() {
  Serial.begin(115200);

  EPSerial.begin(9600, SERIAL_8N1, EP_RXD, EP_TXD);
  EPSerial.setTimeout(2);

  delay(250);
}

void loop() {
  // reuest data to 2nd Serial
  EPIPDB.request(EPSerial);

  // give EPIPDB some time to answer
  for (int i = 0; i < 100; i++) {
    EPIPDB.handle(EPSerial);
    delay(2);
  }

  // in case a valid frame has been received, show it
  if (EPIPDB.getValidAge() < 1000) {
    EPIPDB.print2serial(Serial);
  }

  delay(1000);
}
