
char myString ="T";

void setup() {
  Serial.begin(9600);
  Serial2.begin(19200);
  
}

void loop() {
  Serial2.write("I LOVE U");
  delay(2000);  
}
