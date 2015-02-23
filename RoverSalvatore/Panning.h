#include <Servo.h> 


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

public:
  //Riceve in entrata il delegate da chiamare durante il panning
  Panning(Servo *panningServo);

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


