#include "Arduino.h"

#ifndef RoverMove_h
#define RoverMove_h
#include "RoverMove.h"
#endif



RoverMove::RoverMove(int speedM1, int directionM1, int speedM2, int directionM2)
{
  m_SpeedM1 = speedM1;
  m_DirectionM1 = directionM1; 
  m_SpeedM2 = speedM2; 
  m_DirectionM2 = directionM2;    
  
  pinMode(m_SpeedM1, OUTPUT);
  pinMode(m_SpeedM2, OUTPUT);
  pinMode(m_DirectionM1, OUTPUT);
  pinMode(m_DirectionM2, OUTPUT);
}

int RoverMove::MoveUntil(bool (*unitlDelegate)(), void (*moveDelegate)(long unsigned int))
{
  //Se il puntatore non è valido esco
  if( 0 == unitlDelegate || 0 ==  moveDelegate) 
  {
    // some error or default action here
    return -1;
  }

  //Chiama il delegate del movimento da eseguire
  while(! (*unitlDelegate)())
  {
    (*moveDelegate)(0);
    
    //NON RIESCO A FARLO FUNZIONARE
    //Funziona se passo un puntatore ad una funzione definita nel main
    //(RoverMove::*moveDelegate)(100);
    
    //delay(15);
  } 

  //OK
  return -1;  
}

void RoverMove::Forward(unsigned long time)
{
  analogWrite (m_SpeedM1,255);
  digitalWrite(m_DirectionM1,LOW);

  analogWrite (m_SpeedM2,255);
  digitalWrite(m_DirectionM2,LOW);

  Serial.println("Avanti");

  delay(time);
}

void RoverMove::Backward(unsigned long time)
{
  analogWrite (m_SpeedM1,255);
  digitalWrite(m_DirectionM1,HIGH);

  analogWrite (m_SpeedM2,255);
  digitalWrite(m_DirectionM2,HIGH);

  Serial.println("indietro");

  delay(time);
}


void RoverMove::Halt (unsigned long time)
{
  analogWrite (m_SpeedM1,0);
  digitalWrite(m_DirectionM1,HIGH);

  analogWrite (m_SpeedM2,0);
  digitalWrite(m_DirectionM2,HIGH);

  Serial.println("fermo");

  delay(time);
}


void RoverMove::Right  (unsigned long time)
{
  analogWrite (m_SpeedM1,255);
  digitalWrite(m_DirectionM1,LOW);

  analogWrite (m_SpeedM2,0);
  digitalWrite(m_DirectionM2,HIGH);

  Serial.println("destra");

  delay(time);
}

void RoverMove::SpinRight  (unsigned long time)
{
  analogWrite (m_SpeedM1,255);
  digitalWrite(m_DirectionM1,LOW);

  analogWrite (m_SpeedM2,255);
  digitalWrite(m_DirectionM2,HIGH);

  Serial.println("destra sul posto");

  delay(time);
}

void RoverMove::Left  (unsigned long time)
{
  analogWrite (m_SpeedM1,0);
  digitalWrite(m_DirectionM1,LOW);

  analogWrite (m_SpeedM2,255);
  digitalWrite(m_DirectionM2,LOW);

  Serial.println("sinistra");

  delay(time);
}

void RoverMove::SpinLeft  (unsigned long time)
{
  analogWrite (m_SpeedM1,255);
  digitalWrite(m_DirectionM1,HIGH);

  analogWrite (m_SpeedM2,255);
  digitalWrite(m_DirectionM2,LOW);

  Serial.println("sinistra sul posto");

  delay(time);
}



