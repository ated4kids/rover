#include <Servo.h> 
#ifndef RoverMove_h
  #define RoverMove_h
  #include "RoverMove.h"
#endif


class UnitTesting
{
  public:
    void testServo(Servo *panningServo);
    void testServo2(Servo *panningServo);
    void Forward (RoverMove *rover, unsigned long int time);
    long GetDistance(UltraSonicSensor *sensor);
    void testAcceleroGY61();
    void testColorSensor(int VCC, int OUT, int S2, int S3, int S0, int S1);
    void testRFID (int RSTPin, int SSPin);
};
