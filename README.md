# Alexa... fist me!


This is a readme file for the **smart socket** device that was developed at the Junction and Electra (Alexa) hackathons by ...

## Setup

### Hardware required:

- NodeMCU with ESP-12E module
- 220VAC to 5VDC converter
- 5V relay capable switching 220VAC - SRD-05VDC-SL-C
- AC current sensor 5A for arduino  - TA12-100
- RFID reader - RC522
- RFID stickers
- Power socket
- Power plug
- Jumper wires
- 2x 1KOhm resistor


### Languages used:

- Python 3
- arduino

### TODOs:
1. Add led screen for self IP display
2. Add remote relay switching capabilities (solely a code gap)
3. Improve the AI aspect of the socket
4. Migrate to ESP32 and enable bluetooth
5. Add UI with Kivy ?
6. More and more and more...

### Notes
1. SendCurrentOnly.ino: Reads the current from the sensor and sends it via WiFi to the host - WORKS GOOD
2. SmarterSocket.ino: Identifies RFID, send it to host via WiFi, receives "approve/decline" reply, activating the relay to allow 220V to the power socket, then reads and sends current measurements via WiFi to the host. WAS NOT TESTED YET
