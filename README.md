# Balloon-PROJECT
The Code is complete with all the sensors included. 

The next thing to do would be to implement the function;
void SendDownToEarth which is commented in the code. 

The idea is to create a function that will store the necessary global variables
from the sensors and store them in a nice way in a buffer to send it back to the ground station via SigBee.
This can be done using the command Serial2.write (maybe Serial2.print) with the wanted data.

After, the only necessary thing to do with the code would be opmitization to make it more efficient and maybe check loops in case some
sensor is not working.
