#include <Arduino.h>
#include <HardwareSerial.h>
#include <SSD1306.h>

#include <EPIPDB.h>
EPIPDB EPIPDB;

#define EP_RXD 25
#define EP_TXD 14
#define OLED_I2C_ADDR 0x3C
#define OLED_SDA 21
#define OLED_SCL 22

HardwareSerial EPSerial(1);
SSD1306 oleddisplay(OLED_I2C_ADDR, OLED_SDA, OLED_SCL);

void setup() {
  Serial.begin(115200);

  EPSerial.begin(9600, SERIAL_8N1, EP_RXD, EP_TXD);
  EPSerial.setTimeout(2);
  
  oleddisplay.init();
  oleddisplay.flipScreenVertically();
  oleddisplay.setTextAlignment (TEXT_ALIGN_LEFT);

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
    char displaybuff[32];
    oleddisplay.clear();
    snprintf(displaybuff, sizeof(displaybuff), "Bat1: %6.2f", EPIPDB.getBattery1Voltage());
    oleddisplay.drawString(0, 0, displaybuff);
    snprintf(displaybuff, sizeof(displaybuff), "Bat2: %6.2f", EPIPDB.getBattery2Voltage());
    oleddisplay.drawString(0, 8, displaybuff);
    snprintf(displaybuff, sizeof(displaybuff), "PVol: %6.2f", EPIPDB.getPanelVoltage());
    oleddisplay.drawString(0, 16, displaybuff);
    snprintf(displaybuff, sizeof(displaybuff), "PAmp: %6.2f", EPIPDB.getPanelCurrent());
    oleddisplay.drawString(0, 24, displaybuff);
    oleddisplay.display();
  }

  delay(1000);
}
