> Dieses Projekt basiert auf der Anleitung auf pigetArduino: https://github.com/pigetArduino/gpsLogger


- Zum anschließen: Arduino Pro oder Pro Mini wählen!
- AtMega 328p 3.3V 8Mhz wählen
- comport wählen
- baudrate auf 9600



GPS Logger
----------

![Wiring GPS](https://github.com/telebotter/gpslogger/raw/master/doc/gpslogger.jpg)


Record gps coordinate in a csv file on an Micro card every 15 seconds (DAYHOURMINUTESSECOND.csv)   
[Example file](https://github.com/telebotter/gpslogger/blob/master/doc/example.csv)

# Videos / Instructables
* How to build it : https://www.youtube.com/watch?v=gYxDJqSLVxQ
* How to use it : https://www.youtube.com/watch?v=96iCh5vVnsw
* Instructables : http://www.instructables.com/id/Simple-GPS-Logger/

# How to convert the track

## Gpsies.com
* Go to http://gpsies.com
* Click on Create a track
* Import the csv file
* Or click on convert to get a gpx track file

## Viking
* Download it here : https://sourceforge.net/projects/viking/
* Go to file
* Retrieve
* Import file with GPS Babel
* Choose (at the end of the list) : universal csv with field structure in first line
* Add a map in layer
* Use Open Street Map (Mapnick) or Bing Aerial


# Components
Use theses keyword to find the components
* Gps module ublox Aircraft model mwc: 8€
* Micro sd card 2go (w/ micro sd card adapter): 4€
* Batteries holder 4AAA on/off : 1€ 
* Arduino mini pro 3.3V : 1,50€
* Passive buzzer 3v:  1€  
* Total : 15.50€   

We are using a micro sd card adapter as a micro sdcard reader but you can also use a micro sd card reader module for this.

# Tools
* FTDI basic breakout usb ttl 3.3 : 6.23€
* Cable 30awg  8-color: 5.37€
 
# Software:
  * Arduino (Programmation): http://arduino.madnerd.org
  * Cura (3D printing): https://ultimaker.com/en/products/cura-software

# 3D models
[See 3D models](https://github.com/pigetArduino/gpsLogger/blob/master/3D/)
```
Tested on Ultimaker 2 GO with PLA
To improve solidity print A in solid
A Infill: Solid
B Infill: Light
```
* Author : Olivier Sarrailh   

# Wiring:
![Wiring GPS](https://github.com/pigetArduino/gpsLogger/blob/master/doc/gpsLogger_wiring.png)
##  Micro SD card Adapter
```
1 --> Not connected
2 --> 12
3 --> GND
4 --> 13
5 --> VCC
6 --> GND
7 --> 11
8 --> 10 (Chip select)
```
##  GPS
```
RX --> 2
TX --> 3

```
## Buzzer
```
+ ---> 9
```

# Batttery pack
You should avoid obstructing the antenna with the battery pack, antenna needs to see the sky(or sattelites)
we are using 4x 1,5V AA Batteries, so 6V inputvoltage powersupply for the system

## Libraries
TinyGPS++ library : https://github.com/mikalhart/TinyGPSPlus

## Flashinstructions
```
GND auf GND
VCC auf VCC
RX auf TX
TX auf RX
DTR auf DTR
CTS bleibt offen
```
mit geschickt hinlegen kann man ohne anzulöten flashen
Prozessor auf ATMega328P (3,3V,8MHz) stellen und richtigen COMport wählen, dann kompilieren u hochladen
