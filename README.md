# EPSolar Dual Battery (EPIPDB) solar charger serial interface library.

![EPIPDB-Com Picture](extras/EPIPDB-Com-20A_top.jpg?raw=true "EPIPDB-Com Picture")

Bejing EPsolar
EPEver EPIPDB-Com 10A 20A Dual Battery Solar RVS/Vessels Controller
EPIPDB-COM-10
EPIPDB-COM-20
https://www.epsolarpv.com/product/64.html

Display used for data verification
https://www.epsolarpv.de/produkte/item/mt-1-display.html?category_id=3

The device is running an ATmega8 as seen on the [top view of the PCB](extras/EPIPDB-Com-20A_PCB_top.jpg).

# Sample
![EPIPDB-Com running with SSD1306 OLED display](extras/EPIPDB-TTGO-SSD1306-sample_legend.jpg?raw=true "EPIPDB-Com running with SSD1306 OLED display")

The above picture shows this library running on a TTGO along with a SSD1306 OLED display.
The source code is provided in [arduinoHardwareSerialAndSSD1306.ino](examples/arduinoHardwareSerialAndSSD1306/arduinoHardwareSerialAndSSD1306.ino).

The relevant components are: 
1. LILYGO®TTGO LORA32 V2.0 ESP32 from [aliexpress](https://de.aliexpress.com/item/32846302183.html)
2. 3A DC-DC 7-28V to 5V converter from [aliexpress](https://de.aliexpress.com/item/32862049641.html)
3. Quad Channel Bi-Directional Logic-Level-Shifter from [aliexpress](https://de.aliexpress.com/item/1821378018.html).

I have been running the another instance of this without any level shifter (only a ~100Ω resitor in series to RX of the ESP) for ~6 months in my car now and the TTGO/ESP32 still works but I would recommend using a level shifter.

# Pinout

Client side:

![EPIPDB-Com RJ45 Pinout](extras/pinout-eagle-schematic_cropped.png?raw=true "EPIPDB-Com RJ45 Pinout")

Client Board (top view!):

![EPIPDB-Com RJ45 Pinout](extras/pinout-eagle-board_cropped.png?raw=true "EPIPDB-Com RJ45 Pinout")

The most relevant pins would be :
1. Supply from both batteries via diodes with ~1V drop
5. EPCOM TX (connect to Arduino RX), caution: 5V level
6. EPCOM RX (connect to Arduino TX)
7. GND
8. GND

The RX and TX pins are protected via diodes inside of the EPIPDB-Com as seen here:
![EPIPDB-Com RJ45 Connector](extras/EPIPDB-Com-RJ45_connector.jpg?raw=true "EPIPDB-Com Inside of the EPIPDB-Com").

# Schematics

[PDF](extras/pinout-eagle-schematic.pdf)

[Eagle SCH](extras/pinout-eagle-schematic.sch)