# oura-panel
A dedicated e-paper dashboard for visualizing Ōura Ring health data.

## Hardware
- [DFRobot FireBeetle 2 ESP32-E](https://www.dfrobot.com/product-2195.html) (or FireBeetle ESP32 with micro-USB)
- [Waveshare 7.3inch 6-Color E-Paper Display](https://a.co/d/09qF06k0) (with e-Paper HAT)
- [Waveshare Universal e-Paper Raw Panel Driver Shield](https://a.co/d/7kTwsXB)
- [3.7V LiPo battery](https://a.co/d/02BPgz2V) (compatible with FireBeetle's JST-PH2.0 connector)

## Wiring
Waveshare -> FireBeetle:
BUSY -> 14/D6
RST  -> 25/D2
DC   -> 22/SCL
CS   -> 13/D7
SCLK -> 18/SCK
DIN  -> 23/MOSI
GND  -> GND
VCC  -> 3V3

## Oura API
https://cloud.ouraring.com/v2/docs
