#include <TinyGPS++.h>
#include "SD.h"
#include"SPI.h"
#include <Wire.h>
#include "SHT31.h"
#include "Seeed_BMP280.h"
#include <MiCS6814-I2C.h>
#include "SparkFunLSM6DS3.h"
#include "RadiationWatch.h"


/////////// DEFINE GLOBAL VARIABLES  ////////////
SHT31           sht31 = SHT31();
MiCS6814        gasSensor;
File            SensorData;
TinyGPSPlus     gps;
LSM6DS3         myIMU( I2C_MODE, 0x6A );
BMP280          bmp280;
RadiationWatch  radiationWatch;
static void     smartdelay(unsigned long ms);
float           temp, hum, pressure, T_pressure, alt_pressure, T_in;
float           ppmCO, ppmNO2, ppmNH3, ppmC3H8, ppmC4H10, ppmCH4, ppmH2, ppmC2H5OH;
double          x1a,y1a,z1a,x1g,y1g,z1g;
const int       chipSelect = BUILTIN_SDCARD;
float           latitude, longitude, alt_gps;
float           UValue, OzoneValue;
long            UVindex;
unsigned int    h, m, s, ddmmyy;
bool            sensorConnected;
float           radiationSV = -1, radiationcpm = -1, radiationError = -1;


void setup() {
    // DIGITAL OUTPUT (not reset Xbee)
    pinMode(21, OUTPUT);    // sets the digital pin 21 as output
    digitalWrite(21, HIGH); // sets the digital pin 21 on
    pinMode(34, OUTPUT);    // sets the led as output
    digitalWrite(34, HIGH); // sets the led on
    // SERIALS INITIALIZATION
    //Serial.begin(9600);           //Connection to the computer. Serial monitor.
    Serial4.begin(9600);          // GPS serial port
    Serial3.begin(19200);         // Sigbee connection. Send data down t Earth.
  
    // INITIALIZE SD CARD
    SD.begin(chipSelect);
    // INITIALIZE SENSORS
    InitializeAll();
    // OPEN DATA ON SD CARD
    SensorData = SD.open("DATA.csv", FILE_WRITE);
    if (SensorData){
      //Serial.println("Writing headers to file...");   
      SensorData.println(" Date and Time; Latitude; Longitude; Altgps (m); T_out (C); Humidity (%); Pressure (Pa); Temp_pres (C); alt_pres (m); CO (ppm); NO2 (ppm); NH3 (ppm); C3H8 (ppm); C4H10 (ppm); H2 (ppm); C2H5OH (ppm); x1a (m/s2); y1a (m/s2); z1a (m/s2); x1aGyro (deg); y1aGyro (deg); z1aGyro (deg); UV index; Ozone; Radiation; Rad_Error ");
      }
}

void loop() {
  // GET VALUES OF SENSORS
  Serial.print()
  GetTemperature();
  GetPressure();
  GetGas();
  GetAccelerometer();
  GetUV();
  GetOzone();
  GetRadiation();
  GPS();
  // OPEN DATA ON SD CARD
  if (SensorData){
    //Serial.println("Writing to SD file...");
    SensorData.print(ddmmyy);
    SensorData.print(' ');
    SensorData.print(h);
    SensorData.print(':');
    SensorData.print(m);
    SensorData.print(':');
    SensorData.print(s);
    SensorData.print(";");
    SensorData.print(latitude,6);
    SensorData.print(";");
    SensorData.print(longitude,6);
    SensorData.print(";");
    SensorData.print(alt_gps);
    SensorData.print(";");

    SensorData.print(temp);
    SensorData.print(";");
    SensorData.print(hum);    
    SensorData.print(";");
    SensorData.print(pressure);
    SensorData.print(";");
    SensorData.print(T_pressure);
    SensorData.print(";");
    SensorData.print(alt_pressure);

    SensorData.print(ppmCO);
    SensorData.print(";");
    SensorData.print(ppmNO2);
    SensorData.print(";");
    SensorData.print(ppmNH3);
    SensorData.print(";");
    SensorData.print(ppmC3H8);
    SensorData.print(";");
    SensorData.print(ppmC4H10);
    SensorData.print(";");
    SensorData.print(ppmCH4);
    SensorData.print(";");
    SensorData.print(ppmH2);
    SensorData.print(";");
    SensorData.print(ppmC2H5OH);
    SensorData.print(";");
    
    SensorData.print(x1a);
    SensorData.print(";");
    SensorData.print(y1a);    
    SensorData.print(";");
    SensorData.print(z1a);
    SensorData.print(";");
    SensorData.print(x1g);
    SensorData.print(";");
    SensorData.print(y1g);    
    SensorData.print(";");
    SensorData.print(z1g);
    SensorData.print(";");
    
    SensorData.print(UVindex);
    SensorData.print(";");

    SensorData.print(OzoneValue);

    SensorData.print(";");
    
    SensorData.print(radiationSV);
    SensorData.print(";");
    SensorData.println(radiationError);   
  } 
  
SensorData.close();
digitalWrite(34, HIGH); // sets the led on
delay(1000);
digitalWrite(34, LOW); // sets the led off
SendDataToEarth();
smartdelay(2000);
SensorData = SD.open("DATA.csv", FILE_WRITE);
}

void GetTemperature(){
  temp = sht31.getTemperature();
  hum = sht31.getHumidity();  
}

void GetPressure(){
  pressure = bmp280.getPressure();
  T_pressure = bmp280.getTemperature(); 
  alt_pressure = bmp280.calcAltitude(pressure);
}

void multigasInit(){
  sensorConnected = gasSensor.begin(); 
  gasSensor.powerOn();
}

void GetGas(){
 if (sensorConnected) {
 ppmCO=gasSensor.measureCO();
 ppmNO2=gasSensor.measureNO2();
 ppmNH3=gasSensor.measureNH3();
 ppmC3H8=gasSensor.measureC3H8();
 ppmC4H10=gasSensor.measureC4H10();
 ppmCH4=gasSensor.measureCH4();
 ppmH2=gasSensor.measureH2();
 ppmC2H5OH=gasSensor.measureC2H5OH();
}
else {
 ppmCO=888;
 ppmNO2=888;
 ppmNH3=888;
 ppmC3H8=888;
 ppmC4H10=888;
 ppmCH4=888;
 ppmH2=888;
 ppmC2H5OH=888;
}
}

void AccelerometerInit(){
  myIMU.begin();
}
void GetAccelerometer(){
  x1a = myIMU.readFloatAccelX();
  y1a = myIMU.readFloatAccelY();
  z1a = myIMU.readFloatAccelZ();
  x1g = myIMU.readFloatGyroX();
  y1g = myIMU.readFloatGyroY();
  z1g = myIMU.readFloatGyroZ();
}

void GetUV(){
    long  sum=0;
    for(int i=0;i<1024;i++)// accumulate readings for 1024 times
    {
        UValue=analogRead(A17);
        sum=UValue+sum;
        delay(2);
    }
    long meanVal = sum/1024;  // get mean value
    UVindex = (meanVal*1000/4.3-83)/21;// get a detailed calculating expression for UV index in schematic files.
}

void GPS(){
    while (Serial4.available()){
      int cgp = Serial4.read();
      if (gps.encode(cgp))
      {       
        latitude = gps.location.lat(); // Latitude in degrees (double)
        longitude = gps.location.lng(); // Longitude in degrees (double)
        ddmmyy = gps.date.value(); // Raw date in DDMMYY format (u32)
        h = gps.time.hour(); // Hour (0-23) (u8)
        m = gps.time.minute(); // Minute (0-59) (u8)
        s = gps.time.second(); // Second (0-59) (u8)
        alt_gps = gps.altitude.meters(); // Altitude in meters (double)
      }
    }  
}

void GetOzone(){
  OzoneValue=analogRead(A19);
}


void GetRadiation(){
  radiationWatch.loop();
}

void RadiationInit(){
  radiationWatch.setup();
  // Register the callbacks.
  radiationWatch.registerRadiationCallback(&onRadiation);
  }

void onRadiation()
{
  radiationSV = radiationWatch.uSvh();
  radiationcpm = radiationWatch.uSvh();
  radiationError = radiationWatch.uSvhError();
}


static void smartdelay(unsigned long ms)
{
  unsigned long start = millis();
  do {
    while (Serial4.available())
      gps.encode(Serial4.read());
  } while (millis() - start < ms);
}

void InitializeAll(){
    sht31.begin();
    bmp280.init();
    multigasInit();
    AccelerometerInit();
    RadiationInit();
}
  


void SendDataToEarth(){
// develop code to send the useful information via the SIGBEE.
// Create buffer with the information and send it via Serial2.
    Serial3.print("Date:");
    Serial3.print(ddmmyy);
    Serial3.print(h);
    Serial3.print(m);
    Serial3.print(s);
    Serial3.write("Lat:");
    Serial3.print(latitude,6);  
    Serial3.write("Lon:");
    Serial3.print(longitude,6);
    Serial3.write("AltGPS:");
    Serial3.print(alt_gps,3);
    Serial3.write("T:");
    Serial3.print(temp);   
    Serial3.write("P:");
    Serial3.print(pressure);
    Serial3.write("Tpres:");
    Serial3.println(T_pressure);
}
