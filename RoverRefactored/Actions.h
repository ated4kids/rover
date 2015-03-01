#include <Servo.h> 


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
struct PanningResult
{
public:
  int Direction;
  int Distance;
};

struct Obstacle
{
public:
  int Direction;
  bool Obstacle;
};

struct Zone
{
public:
  int StartPosition;
  int EndPosition;
  int Count;
};

class Panning
{
private:
  Servo m_panningServo;
  bool m_debug;

public:
  //Riceve in entrata il delegate da chiamare durante il panning
  Panning(bool debugging, Servo *panningServo);

  /* 
   Questa funzione riceve un delegate (puntatore a funzione) come parametro e lo scopo è quello di fare il panning del sensore
   fino a quando si verifica una condizione determinata dal delegate stesso (quando la distanza è maggiore di..., quando ritrova 
   la linea nera,...)
   
   return: torna la posizione a cui ha trovato (-1 se non ha trovato)
   */
  int PanUntil(bool (*unitlDelegate)());
  
  PanningResult ScanForMaxDistance(long (*measureDelegate)());
  int ScanForObstacles(long (*measureDelegate)());
};


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

class RoverMove
{
private:
  int m_SpeedM1;
  int m_DirectionM1; 
  int m_SpeedM2; 
  int m_DirectionM2;
  bool m_debug;

public:

  RoverMove(bool debug, int speedM1, int directionM1, int speedM2, int directionM2);
  int ForwardUntil(bool (*unitlDelegate)());
  int MoveUntil(bool (*unitlDelegate)(), void (*moveDelegate)(long unsigned int));
  //int MoveUntil(bool (*unitlDelegate)(), void (RoverMove::*moveDelegate)(long unsigned int));


  void Forward(unsigned long time);
  void Backward(unsigned long time);
  void Halt (unsigned long time);
  void Right  (unsigned long time);
  void SpinRight  (unsigned long time);
  void Left  (unsigned long time);
  void SpinLeft  (unsigned long time);
};

