#include <MiCS6814-I2C.h>


 MiCS6814 gasSensor;
 bool sensorConnected;
 float ppmCO;
 float ppmNO2;
 float ppmNH3;
 float ppmC3H8;
 float ppmC4H10;
 float ppmCH4;
 float ppmH2;
 float ppmC2H5OH;

 void multigasInit(){
 //initializing the multichannel gas sensor
 // Connect to sensor using default I2C address (0x04)
 // Alternatively the address can be passed to begin(addr)
 sensorConnected = gasSensor.begin();

 if (sensorConnected == true) {
 // Print status message
 Serial.println("Connected to multichannel gas sensor");

 // Turn heater element on
 gasSensor.powerOn();
 } else {
 // Print error message on failed connection
 Serial.println("Couldn’t connect to multichannel sensor");
 }
}


void readingGas(){
 //all outputs for the multichannel gas sensor are floats
 //as indicated in the library
 if (sensorConnected) {
 ppmCO=gasSensor.measureCO();
 ppmNO2=gasSensor.measureNO2();
 ppmNH3=gasSensor.measureNH3();
 ppmC3H8=gasSensor.measureC3H8();
 ppmC4H10=gasSensor.measureC4H10();
 ppmCH4=gasSensor.measureCH4();
 ppmH2=gasSensor.measureH2();
 ppmC2H5OH=gasSensor.measureC2H5OH();

 // Serial.println("Gases from multichannel gas sensor");
 Serial.println(ppmCO);
 Serial.println(ppmNO2);
 Serial.println(ppmNH3);
 Serial.println(ppmC3H8);
 Serial.println(ppmC4H10);
 Serial.println(ppmCH4);
 Serial.println(ppmH2);
 Serial.println(ppmC2H5OH);

}
}


void setup() {
  Serial.begin(9600); //
  multigasInit();

}

void loop() {
  readingGas();// put your main code here, to run repeatedly:

}
