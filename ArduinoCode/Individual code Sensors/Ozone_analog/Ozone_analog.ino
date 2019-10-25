void setup(){

    Serial.begin(9600);
}

void loop()
{
      int sensorValue;
      sensorValue=analogRead(A0);
      Serial.print("The current Ozone ppm is:");
      Serial.print(sensorValue);// get a detailed calculating expression for UV index in schematic files.
      Serial.print("\n");
      delay(5000);

}
