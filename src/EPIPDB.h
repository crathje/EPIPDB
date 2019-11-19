/*
   Copyright 2019 Claas Anders Rathje
   Arduino / C++ Library


   Bejing EPsolar
   EPEver EPIPDB-Com 10A 20A Dual Battery Solar RVS/Vessels Controller
   EPIPDB-COM-10
   EPIPDB-COM-20
   https://www.epsolarpv.com/product/64.html

   Display used for data verification
   https://www.epsolarpv.de/produkte/item/mt-1-display.html?category_id=3


   Permission is hereby granted, free of charge, to any person obtaining a copy of this
   software and associated documentation files (the "Software"), to deal in the Software
   without restriction, including without limitation the rights to use, copy, modify,
   merge, publish, distribute, sublicense, and/or sell copies of the Software, and to
   permit persons to whom the Software is furnished to do so, subject to the following
   conditions:

   The above copyright notice and this permission notice shall be included in all
   copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
   PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
   HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
   OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
   SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef epsolar_h_
#define epsolar_h_


#if defined(ESP8266) || defined(ESP31B) || defined(ESP32)
#include <HardwareSerial.h>
#else
#include <iostream>
#include <istream>
#include <ostream>
#include <chrono>
#endif


#if defined(ARDUINO)
#define _ISTREAM Stream
#define _OSTREAM Stream
#define _MILLIS millis()
#define _EPDEBUGSTREAM Serial
#elif __linux__
#define _ISTREAM istream
#define _OSTREAM ostream
#define _MILLIS system_clock::now().time_since_epoch()
#define _EPDEBUGSTREAM cout
#else
#define _ISTREAM istream
#define _OSTREAM ostream
#define _MILLIS  -1
#define _EPDEBUGSTREAM cout
#endif


// datagram should be smaller than this
#define _EPINBUFFSIZE 50
#ifndef _EPDEBUG
#define _EPDEBUG 0
#endif


struct __attribute__((__packed__)) solardata_t
{
  uint8_t a00;
  uint8_t a01;
  uint8_t a02;
  uint8_t a03;
  uint8_t a04;
  uint8_t a05;
  uint8_t a06;
  uint8_t a07;
  uint8_t datalen;              // 8
  uint16_t bat1volt;            // 9
  uint16_t bat2volt;            // 11
  uint16_t panelcurrent;        // 13
  uint16_t panelvolt;           // 15
  uint8_t termperature;         // 17 (35 offset)
  uint8_t externalTermperature; // 18 (35 offset), equals temperature when no external probe connected
  uint8_t a19;
  uint8_t a20;
  uint8_t batterytype; // 21 (0 = Sealed battery, 1 = Gel battery, 2 = Flooded battery)
  uint8_t battery1priority; // 22 - e.g. 20 for 20:80 and 70 for 70:30
  uint8_t pwmFrequency;     // 23
  uint8_t a24;
  uint8_t a25;
  uint8_t a26;
  uint8_t a27;
  uint8_t a28;
  uint8_t a29;
  uint8_t a30;
  uint8_t a31;
  uint8_t a32;
};




class EPIPDB {
  private:
    uint8_t ep_buff[_EPINBUFFSIZE];
    uint16_t ep_buffp = 0;
    unsigned long last_valid_packet;
  public:
    solardata_t solardata; // valid data will be held here
    static uint8_t _EP_DATA_HEADER[6];
    static uint8_t _EP_REQUEST_DATA[2];
    float getBattery1Voltage();
    float getBattery2Voltage();
    float getPanelCurrent();
    float getPanelVoltage();
    int8_t getTemperature();
    int8_t getExternalTemperature();
    uint8_t getPWMFrequency();
    uint8_t getChargingPriority();
    // { SEALED = 0, GEL = 1, FLOODED = 2 };
    uint8_t getBatteryType();

    unsigned long getValidAge();

    void print2serial(_OSTREAM& serial);
    void request(_OSTREAM& serial);
    void handle(uint8_t c);
    void handle(_ISTREAM &stream);
    void debug2Serial(_OSTREAM &serial);

  private:
    uint16_t tracerCRCMatch(uint8_t *CRC_Buff, uint8_t crc_len);
};


#endif
