#include "Arduino.h"

#ifndef Actions_h
  #define Actions_h
  #include "Actions.h"
#endif


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
PANNING
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/

//Riceve in entrata il delegate da chiamare durante il panning
Panning::Panning(Servo *panningServo)
{
  m_panningServo = *panningServo;
}


/* 
 Questa funzione va dichiarata prima del punto in cui la si vuole usare
 
 Questa funzione riceve un delegate (puntatore a funzione) come parametro e lo scopo è quello di fare il panning del sensore
 fino a quando si verifica una condizione determinata dal delegate stesso (quando la distanza è maggiore di..., quando ritrova 
 la linea nera,...)
 
 return: torna la posizione a cui ha trovato (-1 se non ha trovato)
 */
int Panning::PanUntil(bool (*unitlDelegate)()) 
{  
  //Se il puntatore non è valido esco
  if( 0 == unitlDelegate ) 
  {
    // some error or default action here
    return -1;
  }

  bool r;

  //porta il servo a 0
  m_panningServo.write(0);
  //dà il tempo al servo per spostarsi
  delay (100);

  int pos;
  //iniza la scansione
  //ev modificare lo step per velocizzare la scansione
  for(pos = 30; pos < 150; pos += 5)
  {
    m_panningServo.write(pos);

    //Chiama il delegate e se torna true significa che ha trovato qualcosa, altrimenti continua
    if ((*unitlDelegate)())
    {
      Serial.print("trovato pos ");
      Serial.println(pos);
      return pos;
    } 
    delay(100);
  } 

  //OK
  return -1;
}


//Esegue la scansione impostando un array con gli ostacoli (<20cm) e determina la via da percorrere in base 
//alla zona libera piu' ampia
int Panning::ScanForObstacles(long (*measureDelegate)())
{
  Serial.println();
  Serial.println();
  
  //Se il puntatore non è valido esco
  if( 0 == measureDelegate ) 
  {
    // some error or default action here
    return -1;
  }

  bool r;

  int start = 20;
  int end = 180;
  int step = 20;
  
  int arrLength = (end-start) / step + 1;
  
  struct Obstacle obstacles[arrLength];
  Serial.print("array: ");
  Serial.println(arrLength);




  //porta il servo a 0
  m_panningServo.write(start);
  //dà il tempo al servo per spostarsi
  delay (500);

  int pos;
  
  int progressive = 0;
  //iniza la scansione
  //ev modificare lo step per velocizzare la scansione
  for(pos = start; pos <= end; pos += step)
  {
    m_panningServo.write(pos);    
    delay(500);
    
    long currentMeasure = (*measureDelegate)();


    Serial.print(progressive);
    Serial.print(", Dist: ");
    Serial.print(currentMeasure);
    Serial.print(", pos: ");
    Serial.println(pos);


    obstacles[progressive].Direction = pos;    

    obstacles[progressive].Obstacle = currentMeasure < 20;    
    
    progressive++;
  } 
  
  bool lastObstacle = true;
  struct Zone foundZone;
  foundZone.StartPosition=-1;
  foundZone.EndPosition=-1;
  foundZone.Count = -1;

  struct Zone maxFoundZone;
  maxFoundZone.StartPosition=-1;
  maxFoundZone.EndPosition=-1;
  maxFoundZone.Count = -1;

  for (int obstaclePos = 0; obstaclePos < arrLength; obstaclePos++)
  {
    //Zone NOT Changed
    if (obstacles[obstaclePos].Obstacle == lastObstacle)
    {
      if (!obstacles[obstaclePos].Obstacle)
      {
        foundZone.EndPosition++;
        foundZone.Count++;
      }
      else
      {
        foundZone.StartPosition=-1;
        foundZone.EndPosition=-1;
        foundZone.Count = -1;
      }
    }
    else
    {
      if (!obstacles[obstaclePos].Obstacle)
      {
        foundZone.StartPosition=obstaclePos;
        foundZone.EndPosition=obstaclePos;
        foundZone.Count = 1;
      }
      else
      {
        if (maxFoundZone.Count < foundZone.Count)
        {
          maxFoundZone = foundZone;
        }  
      }
    }
    
    lastObstacle = obstacles[obstaclePos].Obstacle;
  }
  
  if (maxFoundZone.Count < foundZone.Count)
  {
    maxFoundZone = foundZone;
  }
  
  Serial.print("StartPosition: ");
  Serial.print(maxFoundZone.StartPosition);
  Serial.print(", EndPosition: ");
  Serial.print(maxFoundZone.EndPosition);
  Serial.print(", Count: ");
  Serial.println(maxFoundZone.Count);


  for (int obstaclePos = 0; obstaclePos < arrLength; obstaclePos++)
  {
    if (obstacles[obstaclePos].Obstacle)
      Serial.print(1);
    else
      Serial.print(0);
  }
  Serial.println();
  
  
  int direzione = (obstacles[maxFoundZone.EndPosition].Direction + obstacles[maxFoundZone.StartPosition].Direction) / 2;

  Serial.print("direzione ");
  Serial.println(direzione);  
  
  return direzione;
}

//Cerca il punto che torna la distanza maggiore
PanningResult Panning::ScanForMaxDistance(long (*measureDelegate)())
{
  struct PanningResult ret;
  ret.Distance = -1;
  ret.Direction = -1;
  
  //Se il puntatore non è valido esco
  if( 0 == measureDelegate ) 
  {
    // some error or default action here
    return ret;
  }

  bool r;

  int start = 20;
  int end = 180;
  int step = 20;

  //porta il servo a 0
  m_panningServo.write(start);
  //dà il tempo al servo per spostarsi
  delay (500);

  long maxMeasure = 0;
  int widestPosition = 0;

  int pos;
  //iniza la scansione
  //ev modificare lo step per velocizzare la scansione
  for(pos = start; pos <= end; pos += step)
  {
    m_panningServo.write(pos);    
    delay(500);
    
    long currentMeasure = (*measureDelegate)();


      Serial.print("Dist: ");
      Serial.print(currentMeasure);
      Serial.print(", pos: ");
      Serial.println(pos);

    if (currentMeasure > maxMeasure)
    {
      maxMeasure = currentMeasure;
      widestPosition = pos;

      ret.Distance = currentMeasure;
      ret.Direction = pos;
    }
  } 

  Serial.print("trovato pos ");
  Serial.println(widestPosition);  
  
  return ret;
}


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
ROVER MOVE
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/

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



