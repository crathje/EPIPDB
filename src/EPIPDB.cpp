/* Copyright 2019 Claas Anders Rathje

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

#include "EPIPDB.h"

uint8_t EPIPDB::_EP_DATA_HEADER[] = {0xEB, 0x90, 0xEB, 0x90, 0xEB, 0x90};
uint8_t EPIPDB::_EP_REQUEST_DATA[] = {0xAA, 0x55};



void EPIPDB::request(_OSTREAM& serial)
{
  serial.write(_EP_REQUEST_DATA, sizeof(_EP_REQUEST_DATA));
}


float EPIPDB::getBattery1Voltage() {
  return solardata.bat1volt / 100.0;
}
float EPIPDB::getBattery2Voltage() {
  return solardata.bat2volt / 100.0;
}
float EPIPDB::getPanelCurrent() {
  return solardata.panelcurrent / 100.0;
}
float EPIPDB::getPanelVoltage() {
  return solardata.panelvolt / 100.0;
}

int8_t EPIPDB::getTemperature() {
  return  solardata.termperature - 35;
}
int8_t EPIPDB::getExternalTemperature() {
  return  solardata.externalTermperature - 35;
}

uint8_t EPIPDB::getPWMFrequency() {
  return solardata.pwmFrequency;
}
uint8_t EPIPDB::getChargingPriority() {
  return solardata.battery1priority;
}
uint8_t EPIPDB::getBatteryType() {
  return solardata.batterytype;
}

void EPIPDB::print2serial(_OSTREAM& serial)
{
  serial.printf("valid %lums ago\n", getValidAge());
  serial.printf("bat1volt:            %6.2f\n", getBattery1Voltage());
  serial.printf("bat2volt:            %6.2f\n", getBattery2Voltage());
  serial.printf("panelcurrent:        %6.2f\n", getPanelCurrent());
  serial.printf("panelvolt:           %6.2f\n", getPanelVoltage());
  serial.printf("temperature:         %6d\n", getTemperature());
  serial.printf("externalTemp:        %6d\n", getExternalTemperature());
  serial.printf("batterytype:         %6d\n", getBatteryType());
  serial.printf("battery1priority:    %6d\n", getChargingPriority());
  serial.printf("pwmFrequency:        %6d\n", getPWMFrequency());

}



void EPIPDB::handle(uint8_t c)
{
  if (ep_buffp >= _EPINBUFFSIZE - 1)
  {
    ep_buffp = 0; // failed
    memset(ep_buff, 0, _EPINBUFFSIZE);
  }

  ep_buff[ep_buffp] = c;

  int rc = memcmp(ep_buff, EPIPDB::_EP_DATA_HEADER, ep_buffp > sizeof(EPIPDB::_EP_DATA_HEADER) ? sizeof(EPIPDB::_EP_DATA_HEADER) : ep_buffp);

  // still valid package
  if (rc == 0)
  {
    // ok!
    //debug2Serial(Serial);
    if (ep_buffp > 8 && ep_buffp == ((solardata_t*)(ep_buff))->datalen + sizeof(EPIPDB::_EP_DATA_HEADER) + 5) // 8 is the packet length
    {
      if (tracerCRCMatch(ep_buff + sizeof(EPIPDB::_EP_DATA_HEADER), ((solardata_t*)(ep_buff))->datalen + 5) == 0)
      {
        last_valid_packet = _MILLIS;
        memcpy((void*)&solardata, ep_buff, sizeof(solardata_t));

#if _EPDEBUG > 1
  #if defined(ESP8266) || defined(ESP31B) || defined(ESP32)
        print2serial(_EPDEBUGSTREAM);
  #endif
#endif

      }
      else
      {
        printf("ERROR\n");
      }

      ep_buffp = 0; // done
      memset(ep_buff, 0, _EPINBUFFSIZE);
    }
    else
    {
      ep_buffp++;
    }
  }
  else
  {
    ep_buffp = 0; // failed
    memset(ep_buff, 0, _EPINBUFFSIZE);
  }
}

void EPIPDB::handle(_ISTREAM &stream)
{
  while (stream.available()) {
    handle(stream.read());
  }
}

// https://github.com/xxv/tracer/blob/master/docs/Protocol-Tracer-MT-5.pdf
uint16_t EPIPDB::tracerCRCMatch(uint8_t *CRC_Buff, uint8_t crc_len)
{
  uint8_t crc_i, crc_j, r1, r2, r3, r4;
  uint16_t crc_result;

  r1 = *CRC_Buff;
  CRC_Buff++;
  r2 = *CRC_Buff;
  CRC_Buff++;
  for (crc_i = 0; crc_i < crc_len - 2; crc_i++)
  {
    r3 = *CRC_Buff;
    CRC_Buff++;
    for (crc_j = 0; crc_j < 8; crc_j++)
    {
      r4 = r1;
      r1 = (r1 << 1);
      if ((r2 & 0x80) != 0)r1++;
      r2 = r2 << 1;
      if ((r3 & 0x80) != 0)r2++;
      r3 = (r3 << 1);
      if ((r4 & 0x80) != 0)
      {
        r1 = r1 ^ 0x10;
        r2 = r2 ^ 0x41;
      }
    }
  }
  crc_result = r1;
  crc_result = crc_result << 8 | r2;
  return (crc_result);
}


unsigned long EPIPDB::getValidAge() {
  return _MILLIS - last_valid_packet;
}
void EPIPDB::debug2Serial(Stream& serial) {
  serial.printf("\n\n");
  for (int i = 0; i < _EPINBUFFSIZE; i++) {
    serial.printf("%02d: 0x%02X %02d\n", i, *(((uint8_t*)&ep_buff) + i), *(((uint8_t*)&ep_buff) + i));
  }
}
