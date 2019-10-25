
#include <SD.h>
#include <SPI.h>


#define TXBUFFER_MAX 200

//SoftwareSerial SoftSerial(0, 1);
File  myFile;
unsigned char buffer[664];     // buffer array for data receive over serial port
unsigned char txbuffer[TXBUFFER_MAX];     // buffer array for data to send
int txbuffer_count;

int count=0; // counter for buffer array
const int     chipSelect = BUILTIN_SDCARD;
String  labels = "Time, Status, Latitude, Hemisphere, Longitude, Hemisohere, Speed , Track angle, Date";
float var;



void clearBufferArray()                     // function to clear buffer array
{
    for (int i=0; i<count;i++)
    {
        buffer[i]=NULL;
    }                      // clear all index of array with command NULL
}


  

void setup(){
   Serial.begin(115200);                     // the Serial port of Arduino baud rate.
   Serial1.begin(9600);                     // the GPS Serial port.

  // INITIALIZATION OF SD CARD


/*  
  // if the file opened okay, write to it:*/


}

void loop(){

 // Serial.println("Inside loop\n");
    if (Serial1.available())  {                   // if gps data is not coming from software serial port ==> data is coming from SoftSerial shield
     // Serial.println("Working...\n");
            while(Serial1.available())  {             // reading data into char array      
                buffer[count]=Serial1.read();      // writing data into array
                count++;
            }
            /// WRITE TO SD CARD
            
            File myFile = SD.open("TEST.txt",FILE_WRITE); //create file object and open csv in setup
            File dataFile = SD.open("DATA.txt",FILE_WRITE); //create file object and open csv in setup
            //Serial.println("SD card opened.\n");
            // if file is available write to it
            if (dataFile){
              dataFile.write(buffer,count);
            }
            dataFile.close();
            myFile.close();/// CLOSE SD CARD
            Serial.write(buffer,count);

            
            //delay(500);
            //Serial.write(buffer,count);

            }
            /*write_antenna_buffer(buffer,10);                      // 10th bit- 15 th bit 
            Serial.println("txbuffer:");
            Serial.write(txbuffer,100);
            Serial.println(txbuffer_count);
            Serial.println("send:");
            send_antenna_buffer();*/
            //clearBufferArray();                         // call clearBufferArray function to clear the stored data from the array
            count = 0;                                  // set counter of while loop to zero
       
    //else{
      //      Serial.println("Software serial not opened\n");
    //}
   if (Serial.available())                 // if data is available on hardware serial port ==> data is coming from PC or notebook
   Serial1.write(Serial.read());        // write it to the SoftSerial shield
}
