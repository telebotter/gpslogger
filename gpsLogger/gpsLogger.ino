#include <TinyGPS++.h>
#include <SoftwareSerial.h>
// SD
#include <SPI.h>
#include <SD.h>
//math, cos atan2 etc
#include <math.h>

//milli Seconds between save
const int gps_interval = 30 * 1000;

// Alles immer in UTC Time
//GPS TX/RX (don't use default TX/RX)
const int RXPin = 2;
const int TXPin = 3;

// Ublox run at 9600
const int GPSBaud = 9600;

//SD
const int chipSelect = 10;

bool locationOK = false; //Is GPS connected ?
bool sdOK = true; //Is SD card detected ?

String string_filename;
char* filename;

/* Formatted Date and Time */
String s_day;
String s_month;
String s_year;
String s_hour;
String s_minute;
String s_second;

double latitude_old=0;
double longitude_old=0;
double latitude=0;
double longitude=0;
double angle = 0;
double angle_old = 0;

// Create a TinyGPS++ object called "gps"
TinyGPSPlus gps;

// Create a software serial port called "gpsSerial"
// We can't use TX/RX pins if we don't want to lose the ability to debug/upload code
SoftwareSerial gpsSerial(TXPin, RXPin);

void setup() {
  // Start Serial debug
  Serial.begin(9600);

  // Start GPS Serial
  gpsSerial.begin(GPSBaud);

  // Start SD Card
  if (!SD.begin(chipSelect)) {
    Serial.println("SD : ERROR");
    sdOK = false;
    // don't do anything more:
    return;
  }
  Serial.println("SD : OK");
}

void loop() {
  // When GPS send coordinates
  while (gpsSerial.available() > 0)
    if (gps.encode(gpsSerial.read())){
      if (gps.location.isUpdated()){
        formatData(); //Format data
        //displayInfo(); //Display debug
        angle_old = angle;
        latitude_old = latitude;
        longitude_old = longitude;
        
        saveToSD(); //Save in SDCard - write longitude and latitude
        
        angle = (3*direction(latitude_old, longitude_old,latitude,longitude)+ angle_old)/4; //weighted running mean
        // if button was pressed, dest_direction = angle - direction(latitude,longitude,latitude_dest,longitude_dest)
        // welche LED soll leuchten? led_to_light = dest_direction +(360/num_led/2)+180)//(360/num_led))%num_led
        
        //We make the arduino wait
        delay(gps_interval);
      }
      else{
        delay(1000);
      }
    }

  // Check if GPS is OK
  if (millis() > 5000 && gps.charsProcessed() < 10) {
  // If 5000 milliseconds pass and there are no characters coming in
  // over the software serial port.
    Serial.println(F("GPS : ERROR"));
  }

}
//gives absolute angle (nosw) between two points
double direction(double latitude_0,double longitude_0,double latitude_1, double longitude_1) {
  double dy = latitude_1 - latitude_0;
  double dx = cos(3.14159265358979/180 *latitude_0)*(longitude_1 - longitude_0);
  double angle = atan2(dy, dx)/ (2*3.14159265358979)*360;
  return angle;
}
//SD : Write Header for CSV file
void writeHeaders() {
  Serial.println("Writing header");
  Serial.println(filename);
  File gpsFile = SD.open(filename, FILE_WRITE);
  if (gpsFile) {
    gpsFile.println("Latitude,Longitude,Alt,Date,Time,Speed");
  } else {
    Serial.println("SD : ERROR WRITING");
  }
  gpsFile.close();
}

//Format data
void formatData(){
   /* Format Date */
  // Year
  s_year = gps.date.year();

  // Day
  if (gps.date.day() < 10) {
    s_day = "0" + String(gps.date.day());
  } else {
    s_day = String(gps.date.day());
  }

  // Month
  if (gps.date.month() < 10) {
    s_month = "0" + String(gps.date.month());
  } else {
    s_month = String(gps.date.month());
  }

  // Hour
  // UTC

  if (gps.time.hour() < 10) {
    s_hour = "0" + String(gps.time.hour());
  } else {
    s_hour = String(gps.time.hour());
  }

  // Minutes
  if (gps.time.minute() < 10) {
    s_minute = "0" + String(gps.time.minute());
  } else {
    s_minute = String(gps.time.minute());
  }

  // Seconds
  if (gps.time.second() < 10) {
    s_second = "0" + String(gps.time.second());
  } else {
    s_second = String(gps.time.second());
  }
}

/* Display debug information */
void displayInfo() {
  //Date
  Serial.print(s_day);
  Serial.print(F("/"));
  Serial.print(s_month);
  Serial.print(F("/"));
  Serial.print(s_year);
  Serial.print(" ");

  //Time
  Serial.print(s_hour);
  Serial.print(F(":"));
  Serial.print(s_minute);
  Serial.print(F(":"));
  Serial.print(s_second);

  //Lat / Long / Altitude / Speed
  Serial.print(F(","));
  Serial.print(gps.location.lat(), 6);
  Serial.print(F(","));
  Serial.print(gps.location.lng(), 6);
  Serial.print(F(","));
  Serial.print(gps.altitude.meters(), 6);
  Serial.print(F(","));
  Serial.print(gps.speed.kmph());
  Serial.println();
}

void saveToSD() {
  //Check if SD card works or stop
  if (sdOK) {
    /* Create file when gps found a position */
    //If GPS didn't find position (latitude = 0)
    if (! locationOK) {
      if (gps.location.lat() != 0)
      {
        //Create filename
        string_filename =s_day + s_hour + s_minute + s_second + ".csv";

        //Convert filename string to char
        unsigned int bufSize = string_filename.length() + 1; //String length + null terminator
        filename = new char[bufSize];
        string_filename.toCharArray(filename, bufSize);

        //GPS found
        locationOK = true;
        
        //Display filename
        Serial.println("FILENAME");
        Serial.println(filename);

        //Write first line of CSV
        writeHeaders();

      }
    }

    //Append position to file each 15 minutes (by default)
    if (locationOK) {
      //Check if GPS hasn't lost position (it will keep last position but reset date)
      if (gps.date.year() != 2000) {

        //Buzz to tell the user data is written
        Serial.println("APPEND");

        //Open file
        File gpsFile = SD.open(filename, FILE_WRITE);

        if (gpsFile) {
          //Latitude / Longitude       
          double latitude = gps.location.lat();
          double longitude = gps.location.lng();
          gpsFile.print(gps.location.lat(), 6);
          gpsFile.print(F(","));
          gpsFile.print(gps.location.lng(), 6);
          
          //Altitude (meters) (doesn't seems to work)
          gpsFile.print(F(","));
          gpsFile.print(gps.altitude.meters(), 6);

          //Date 2003/06/29
          gpsFile.print(F(","));
          gpsFile.print(s_year);
          gpsFile.print(F("/"));
          gpsFile.print(s_month);
          gpsFile.print(F("/"));
          gpsFile.print(s_day);

          //Time 09:00:00
          gpsFile.print(",");
          gpsFile.print(s_hour);
          gpsFile.print(F(":"));
          gpsFile.print(s_minute);
          gpsFile.print(F(":"));
          gpsFile.print(s_second);

          //Speed (km/h)
          gpsFile.print(F(","));
          gpsFile.print(gps.speed.kmph());
          gpsFile.println();
          
        } else {
          Serial.println("SD : ERROR WRITING");
        }
        gpsFile.close(); //Close file
      }
    } 
  }
}
