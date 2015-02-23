#include "Arduino.h"
#ifndef ColorSensor_h
#define ColorSensor_h
#include "ColorSensor.h"
#endif


const int  Filters_R = 0;
const int  Filters_G = 1;
const int  Filters_B = 2;


RGB ColorSensor::GetRGB()
{
  struct RGB ret;
  
  ret.R = -1;
  ret.G = -1;
  ret.B = -1;
  
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++)
  {
    RGBFilter=i;
    Select_Filters(i);
    count=0;
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
//    Serial.print("value: ");
//    Serial.println(scaleFactor[i]*count);
    
    if (i == 0)
    {
      ret.R = scaleFactor[i]*count;
//      Serial.print("r value: ");
//      Serial.println(scaleFactor[i]*count);
    }
    else if (i == 1)
    {
      ret.G = scaleFactor[i]*count;
//      Serial.print("g value: ");
//      Serial.println(scaleFactor[i]*count);
    }
    else if (i == 2)
    {
      ret.B = scaleFactor[i]*count;
//      Serial.print("b value: ");
//      Serial.println(scaleFactor[i]*count);
    }
    else 
    { 
      Serial.println("Errore");
    }
    
    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);
  digitalWrite(S2, LOW);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);
  digitalWrite(S1, LOW);  
  digitalWrite(S1, HIGH);
      
  return ret;
}

ColorSensor::ColorSensor(int vcc, int out, int s2, int s3, int s0, int s1)
{
  VCC = vcc;
  OUT = out;
  S2  = s2;
  S3  = s3;
  S0  = s0;
  S1  = s1;
  
  RGBFilter = 0;
  count = 0;
  counter = 0;
  G_flag = 1;

  pinMode(OUT, INPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
 
  digitalWrite(S2, HIGH);
  digitalWrite(S3, LOW);
  digitalWrite(S0, LOW);  // OUTPUT FREQUENCY SCALING 2%
  digitalWrite(S1, HIGH);
  
  digitalWrite(VCC, HIGH);
  delay(100);
  
  for(int i=0; i<3; i++) //R=0, G=1, B=2
  {
    Select_Filters(i);
    int lastDigitalRead = HIGH;
    for(int j=0; j<20000; j++)
    {
      int digitalReadValue = digitalRead(OUT);
      if (lastDigitalRead == LOW && digitalReadValue == HIGH) 
      {
        count++;
      }
      counter++;
      lastDigitalRead = digitalReadValue;
    }
    
    scaleFactor[i] = 255.0/count;

    counter=0;
    count=0;
  }
  
  digitalWrite(VCC, LOW);
}

void ColorSensor::Select_Filters(int RGB)
{
  switch(RGB)
  {
    case Filters_R:          //Red
    digitalWrite(S2, LOW);    
    digitalWrite(S3, LOW);
    break;
    
    case Filters_G:          //Green
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, HIGH);
    break;
    
    case Filters_B:          //Blue
    digitalWrite(S2, LOW);    
    digitalWrite(S3, HIGH);
    break;
    
    default:                  //Clear(no filter)
    digitalWrite(S2, HIGH);    
    digitalWrite(S3, LOW);
    break;
  }
}
