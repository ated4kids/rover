#include "Arduino.h"

#ifndef UltraSonicSensor_h
#define UltraSonicSensor_h
#include "UltraSonicSensor.h"
#endif


UltraSonicSensor::UltraSonicSensor (int pingpin, int echopin)
{
  m_pingpin = pingpin;
  m_echopin = echopin;
}

long UltraSonicSensor::GetDistance()
{
  // establish variables for duration of the ping, 
  // and the distance result in inches and centimeters:
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(m_pingpin, OUTPUT);
  digitalWrite(m_pingpin, LOW);
  delayMicroseconds(2);
  digitalWrite(m_pingpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(m_pingpin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(m_echopin, INPUT);
  duration = pulseIn(m_echopin, HIGH);

  // convert the time into a distance
  cm = microsecondsToCentimeters(duration);
  
  //Serial.print("dist ");
  //Serial.println(cm);

  return cm;
}

long UltraSonicSensor::microsecondsToInches(long microseconds)
{
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 74 / 2;
}

long UltraSonicSensor::microsecondsToCentimeters(long microseconds)
{
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 29 / 2;
}



