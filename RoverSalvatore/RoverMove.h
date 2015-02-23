class RoverMove
{
private:
  int m_SpeedM1;
  int m_DirectionM1; 
  int m_SpeedM2; 
  int m_DirectionM2;

public:

  RoverMove(int speedM1, int directionM1, int speedM2, int directionM2);
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
