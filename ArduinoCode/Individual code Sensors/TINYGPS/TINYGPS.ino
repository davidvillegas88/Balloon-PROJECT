#include <TinyGPS++.h>
#include "SD.h"
#include"SPI.h"
#include <Wire.h>

TinyGPSPlus gps;
static void smartdelay(unsigned long ms);
String gpsString =""; // holds the data to be written to the SD card
File gpsFile;
double latitude, longitude, alt;
unsigned int h, m, s, ddmmyy;
const int     chipSelect = BUILTIN_SDCARD;



void setup()
{
  Serial.begin(9600);
  //Serial.println("Sats HDOP Latitude  Longitude  Fix  Date       Time     Date Alt    Course Speed Card  Distance Course Card  Chars Sentences Checksum");
  //Serial.println("          (deg)     (deg)      Age                      Age  (m)    --- from GPS ----  ---- to London  ----  RX    RX        Fail");
  //Serial.println("-------------------------------------------------------------------------------------------------------------------------------------");
  Serial1.begin(9600);
  Serial2.begin(19200);

  if (!SD.begin(chipSelect)) {  // see if the card is present and can be initialized:
    Serial.println("Card failed, or not present");
  }
  Serial.println("card initialized.");  // create a new file
}


void loop()
{
  //Serial.println("Working... ");
  //if (Serial1.available()){
    //Serial.println("Working2... ");
    while (Serial1.available()){
      //Serial.println("Working3... ");
      gps.encode(Serial1.read());
      if (gps.location.isUpdated())
      {        
        latitude = (gps.location.lat(), 6); // Latitude in degrees (double)
        longitude = (gps.location.lng(), 6); // Longitude in degrees (double)
        ddmmyy = gps.date.value(); // Raw date in DDMMYY format (u32)
        h = gps.time.hour(); // Hour (0-23) (u8)
        m = gps.time.minute(); // Minute (0-59) (u8)
        s = gps.time.second(); // Second (0-59) (u8)
        alt = gps.altitude.meters(); // Altitude in meters (double)
        //Serial.println(latitude); // Latitude in degrees (double)
        //Serial.println(longitude); // Longitude in degrees (double)
        //Serial.println(ddmmyy); // Raw date in DDMMYY format (u32)
        //Serial.println(h); // Hour (0-23) (u8)
        //Serial.println(s); // Minute (0-59) (u8)
        //Serial.println(m); // Second (0-59) (u8)
        //Serial.println(alt); // Altitude in meters (double)
        //gpsString = latitude + "," + longitude + "," + alt + "," + h + "," + m + "," + ) + "," + ddmmyy; // convert to CSV

        //gpsString = String(latitude) + "," + String(longitude) + "," + String(alt) + "," + String(h) + "," + String(m) + "," + String(s) + "," + String(ddmmyy); // convert to CSV
  
        //Serial.println(gpsString);


        gpsFile = SD.open("gpsData.txt", FILE_WRITE);
        Serial.println("Working3... ");
        if (gpsFile){
          //gpsFile.println(gpsString);
          Serial.println("Working4... ");
          gpsFile.print(ddmmyy);
          gpsFile.print(' ');
          gpsFile.print(h);
          gpsFile.print(':');
          gpsFile.print(m);
          gpsFile.print(':');
          gpsFile.print(s);
          gpsFile.print(",");
          gpsFile.print(latitude);
          gpsFile.print(",");
          gpsFile.print(longitude);
          gpsFile.print(",");
          gpsFile.print(alt);
          gpsFile.println(' ');
          gpsFile.close();  //close the file
          }
        else{
          Serial.println("Error writing to file !");
        }
        Serial2.print(latitude);
        Serial2.print(",");
        Serial2.println(longitude);
    }
  


smartdelay(10000);
    }

}

static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    while (Serial1.available())
      gps.encode(Serial1.read());
  } while (millis() - start < ms);
}
