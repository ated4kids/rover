//#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

/*

VCC su 5v
GND su ground
SCL su SCL
SDA su SDA


ATTENZIONE : la classe si chiama accelerometer ma di fatto si tratta di un compass

*/

class Accelerometer
{
public:
  Accelerometer();
  float ReadPosition();
};
