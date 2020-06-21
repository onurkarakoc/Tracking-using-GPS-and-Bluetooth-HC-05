#include <TinyGPS++.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(6,5);
/* Create object named bt of the class SoftwareSerial */
SoftwareSerial GPS_SoftSerial(4, 3);/* (Rx, Tx) */
/* Create an object named gps of the class TinyGPSPlus */
TinyGPSPlus gps; 

volatile float minutes, seconds;
volatile int degree, secs, mins;

void setup() {
  Serial.begin(9600); //open the serial port
  mySerial.begin(9600); // open the bluetooth serial port
  GPS_SoftSerial.begin(9600); /* Define baud rate for software serial communication */
}

void loop() {
  smartDelay(1000);
  double lat_val, lng_val;
  bool loc_valid;

  lat_val = gps.location.lat(); /* Get latitude data */
  loc_valid = gps.location.isValid(); /* Check if valid location data is available */
  lng_val = gps.location.lng(); /* Get longtitude data */

  if (!loc_valid)
        {          
          Serial.print("Latitude : ");
          Serial.println("*****");
          Serial.print("Longitude : ");
          Serial.println("*****");
          mySerial.print(0, 3);
          mySerial.print("|");
          mySerial.println(0, 3);
        }
   else
        {
          
          DegMinSec(lat_val);
          mySerial.print(lat_val, 4);
          mySerial.print("|");
          DegMinSec(lng_val);
          mySerial.println(lng_val, 4);
        }
}

static void smartDelay(unsigned long ms)
{
  unsigned long start = millis();
  do 
  {
    while (GPS_SoftSerial.available())  /* Encode data read from GPS while data is available on serial port */
      gps.encode(GPS_SoftSerial.read());
/* Encode basically is used to parse the string received by the GPS and to store it in a buffer so that information can be extracted from it */
  } while (millis() - start < ms);
}
void DegMinSec(double tot_val)   /* Convert data in decimal degrees into degrees minutes seconds form */
{  
  degree = (int)tot_val;
  minutes = tot_val - degree;
  seconds = 60 * minutes;
  minutes = (int)seconds;
  mins = (int)minutes;
  seconds = seconds - minutes;
  seconds = 60 * seconds;
  secs = (int)seconds;
}
