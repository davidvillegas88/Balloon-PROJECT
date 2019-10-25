#include "RadiationWatch.h"
/*
This simple example tell you on serial each time a gamma radiation hits the
Pocket Geiger and print the current micro Sievert dose. It also notify you of
the presence of vibration that prevent proper measurements.
*/

RadiationWatch radiationWatch;
float radiationSV = -1;
float radiationcpm = -1;
float radiationError = -1;
void onRadiation()
{
  Serial.println("A wild gamma ray appeared");
  radiationSV = radiationWatch.uSvh();
  //Serial.print(" uSv/h +/- ");
  radiationcpm = radiationWatch.uSvh();
  radiationError = radiationWatch.uSvhError();
}

void onNoise()
{
  Serial.println("Argh, noise, please stop moving");
}

void setup()
{
  Serial.begin(9600);
  radiationWatch.setup();
  // Register the callbacks.
  radiationWatch.registerRadiationCallback(&onRadiation);
  radiationWatch.registerNoiseCallback(&onNoise);
}

void loop()
{
  radiationWatch.loop();
  Serial.println("SV = ");
  Serial.println(radiationSV);
  //Serial.println(radiationcpm);
  Serial.println("nOISE");
  Serial.println(radiationError);
  delay(3000);
  
}
