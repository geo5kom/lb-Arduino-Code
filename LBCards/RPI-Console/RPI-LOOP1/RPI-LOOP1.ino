/////////////////////////////////////////////////////////////////////////
// RPI-LOOP1 - Receive a character add 1 and return character
/////////////////////////////////////////////////////////////////////////

#include <SoftwareSerial.h>

SoftwareSerial mySerial(2,3); // RX, TX

/////////////////////////////////////////////////////////////////////////
// setup()
/////////////////////////////////////////////////////////////////////////

void setup() 
{
  mySerial.begin(9600);
}

/////////////////////////////////////////////////////////////////////////
// loop()
/////////////////////////////////////////////////////////////////////////

void loop()
{
  if (mySerial.available() > 0)
  {
    mySerial.write(mySerial.read());
  }
}
