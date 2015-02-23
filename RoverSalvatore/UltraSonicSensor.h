class UltraSonicSensor
{
public:
  UltraSonicSensor (int pingpin, int echopin);
  long GetDistance();

private: 
  long microsecondsToInches(long microseconds);
  long microsecondsToCentimeters(long microseconds);

  int m_pingpin;
  int m_echopin;
};



