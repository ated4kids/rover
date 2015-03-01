#include "Arduino.h"

#include <Servo.h> 

#ifndef Sensors_h
  #define Sensors_h
  #include "Sensors.h"
#endif

#ifndef Actions_h
  #define Actions_h
  #include "Actions.h"
#endif

#ifndef UnitTesting_h
  #define UnitTesting_h
  #include "UnitTesting.h"
#endif


void UnitTesting::testAcceleroGY61()
{
  int x=0;
  int y=0;
  int z=0;
  
  x = analogRead(A0);
  y = analogRead(A1);
  z = analogRead(A2);
  Serial.print(x, DEC);
  Serial.print(" ");
  Serial.print(y, DEC);
  Serial.print(" ");
  Serial.println(z, DEC);

 delay(500); //make it readable

}

void UnitTesting::testServo(Servo *panningServo)
{
  int pos;
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    panningServo->write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 

  for(pos = 180; pos>=1; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    panningServo->write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
}


void UnitTesting::testServo2(Servo *panningServo)
{
  panningServo->write(0);
  delay(1000);
  panningServo->write(180);
  delay(1000);
}

//void testForwardUntil()
//{
//  int position = rover->MoveUntil(UntilDelegate, Forward);
//  //rover->Halt(2000);
//}


void UnitTesting::Forward (RoverMove *rover, unsigned long int time)
{
  rover->Forward(time);
}

long UnitTesting::GetDistance(UltraSonicSensor *sensor)
{
  Serial.println(sensor->GetDistance()); 
//  Serial.println(ultraSonic2->GetDistance()); 

}

void UnitTesting::testColorSensor(int vcc, int out, int s2, int s3, int s0, int s1)
{
  ColorSensor* colorSensor = new ColorSensor(vcc, out, s2, s3, s0, s1);
  
  struct RGB rgb;
  rgb = colorSensor->GetRGB();

  Serial.print(rgb.R); 
  Serial.print(", "); 
  Serial.print(rgb.G); 
  Serial.print(", "); 
  Serial.println(rgb.B); 
}

void UnitTesting::testRFID (int RSTPin, int SSPin)
{
  //Da implementare: se lo si fa cosi (creando sempre una istanza) torna sempre dei valori differenti
  
  
//  RFIDReader* rfidReader = new RFIDReader(RSTPin, SSPin);
//  long id = rfidReader->GetUID();
//  Serial.println(id);
}
  
