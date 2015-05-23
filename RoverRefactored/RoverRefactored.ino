#include <Servo.h> 

//#include <AcceleroMMA7361.h>

#ifndef Sensors_h
  #define Sensors_h
  #include "Sensors.h"
#endif

#ifndef Actions_h
  #define Actions_h
  #include "Actions.h"
#endif

#ifndef UnitTesting_h
  #define UnitTesting_h
  #include "UnitTesting.h"
#endif


#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
//#include <HMC5883L.h> //TODO DL UNABLE TO FIN THE LIBRARY 


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
COSTANTI
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
const int servoSignalPin = 2;

//const int pingPinSensor1 = 9;
//const int echoPinSensor1 = 9;

const int pingPinSensor2 = 8;
const int echoPinSensor2 = 8;

const int SpeedM1 = 6; //M1 Speed Control
const int DirectionM1 = 8; //M1 Direction Control

const int SpeedM2 = 5; //M2 Speed Control
const int DirectionM2 = 7; //M2 Direction Control

const int LineTrackingSensor = 9;

const long DISTANCE_TRESHOLD = 30;

const int colorSensorPin = 2;

//Color Sensor constants
const int VCC = 12;
const int OUT = 10;
const int S2  = 8;
const int S3  = 9;
const int S0  = 6;
const int S1  = 7;

//RFID Reader
const int RST_PIN = 9; //9;
const int SS_PIN = 10; //10;

const bool DEBUG = true;

/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
VARIABILI GLOBALI
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
Servo *panningServo;
ColorSensor *colorSensor;
UltraSonicSensor *ultraSonic1;
UltraSonicSensor *ultraSonic2;
RoverMove *rover;
Panning *pan;
RFIDReader *rfidReader;
UnitTesting* testing;
//Compass* compass;//TODO DL UNABLE TO FIND THE LIBRARY FOR THE COMPASS

bool left;

bool DebugMode = true;


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
SETUP
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
void setup() {
/*//TODO DL ALTERNATIVE A FOR SETUP
  // initialize serial communication:
  Serial.begin(9600);

  colorSensor = new ColorSensor(DEBUG, VCC, OUT, S2, S3, S0, S1);

  //Inizializzo tutti gli oggetti che mi servono
  panningServo = new Servo();
  panningServo->attach(servoSignalPin);  
  
  //Centra il servo  
  panningServo->write(90);
  delay(500);

  //Inizializzazione delle varie classi
  ultraSonic2 = new UltraSonicSensor(DEBUG, pingPinSensor2, echoPinSensor2);
  rover = new RoverMove(DEBUG, SpeedM1, DirectionM1, SpeedM2, DirectionM2);
  pan = new Panning(DEBUG, panningServo);
  rfidReader = new RFIDReader(DEBUG, RST_PIN, SS_PIN);

  //Sul rover non c'é l'I2C
  //compass = new Compass(DEBUG);
  
  //Inizializzazione classe di testing
  testing = new UnitTesting();
  
  //Inizializzazione variabili line follower
  pinMode(LineTrackingSensor, INPUT);
  left = true;*/
}


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
LOOP
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
void loop()
{
  //TODO DL ALTERNATIVE A FORLOOP
  Test();
  
  
  /*
  *******************************************************
  *******************************************************
  LOGICHE ROVER
  *******************************************************
  *******************************************************
  */
  //UltraSonicRover();
  
  //LineFollower();  
}


void Test()
{
  //Movimento rover
//  rover->Forward(1200);
//  rover->Halt(3000);

    //Color Sensor
//  struct RGB rgb;
//  rgb = colorSensor->GetRGB();
//  Serial.print("R: "); 
//  Serial.print(rgb.R); 
//  Serial.print(", G: "); 
//  Serial.print(rgb.G); 
//  Serial.print(", B: "); 
//  Serial.println(rgb.B); 
  
  //RFID
  long id = rfidReader->GetUID();

  //Compass
  //Serial.println(compass->ReadPosition());
  //delay(200);


  //Color Sensor
  //testing->testColorSensor(VCC, OUT, S2 S3, S0, S1);

  //Ultrasonic Sensor
  //testing->GetDistance(ultraSonic2);
  //delay(100);
}

/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
ULTRASONIC ROVER
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
void UltraSonicRover()
{
    rover->Halt(1);
    //FindFirstOpenSpace();
    //FindWidestOpenSpace();
    FindWay();
    rover->Forward(1200);
  
  
  
  //bool obstacle = ultraSonic2->GetDistance() < 20;

  //if (obstacle)
  //{
  //  rover->Halt(1);
  //  //FindFirstOpenSpace();
  //  FindWidestOpenSpace();
  //}
  //else
  //{
  //  rover->Forward(100);
  //}
}

/*
 Il delegate va dichiarato prima dei punti dove viene chiamato
 In questa funzione va implementato il meccanismo specifico che torna true in una specifica condizione alla funzione di panning 
 */
bool OpenSpaceDelegate() 
{
  long distance = ultraSonic2->GetDistance();
  bool openspace = distance > DISTANCE_TRESHOLD;

  if (openspace)
    Serial.println(distance);
  
  //Serial.println("dist " + dist);
  return openspace;
}


/*
 Il delegate va dichiarato prima dei punti dove viene chiamato
 In questa funzione va implementato il meccanismo specifico che torna la distanza misurata 
 */
long MeasureDelegate() 
{
  int dist = ultraSonic2->GetDistance(); 
  
  return dist;
}

void FindWay()
{
  int result = pan->ScanForObstacles(MeasureDelegate);
  MoveToPosition(result);  
}

void FindWidestOpenSpace()
{
  PanningResult result = pan->ScanForMaxDistance(MeasureDelegate);
  MoveToPosition(result.Direction);  
}

void FindFirstOpenSpace()
{
  int position = pan->PanUntil(OpenSpaceDelegate);
  MoveToPosition(position);
}

void MoveToPosition(int position)
{
  panningServo->write(90);
  delay(500);
  
  double timeFor90 = 650.0;
  double time = timeFor90 * ( (double)abs((double)position - 90.0) / 90.0);
  
  //Serial.println(position);
  
  if (position >= 0)
  {
    if (position > 90)
    {
      //Serial.println("Trovato a destra");
      rover->SpinLeft(time);
    }
    else
    {
      //Serial.println("Trovato a sinistra");
      rover->SpinRight(time);
    }
  }
  
  rover->Halt(10);
  //delay(5000); 

  if (DebugMode)
    WaitForSerial();
}


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
LINE FOLLOWER
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/
void LineFollower()
{
  int value = digitalRead(LineTrackingSensor);
  
  Serial.println(value);
  
  if (value == LOW && left) //LOW == sono sulla riga
  {
    left = true;
    rover->Left(5);
  }
  else if (value == LOW && !left) //LOW == sono sulla riga
  {
    left = false;
    rover->Right(5);
  }
  else if (value == HIGH && !left)
  {
    left = true;
    rover->Left(5);
  }
  else if (value == HIGH && left)
  {
    left = false;
    rover->Right(5);
  }
}



/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
SERIAL DEBUGGER
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/

void WaitForSerial()
{
  while (true)
  {
  // send data only when you receive data:
   if (Serial.available() > 0) 
   {
     // read the incoming byte:
     byte incomingByte = Serial.read();
     return;
   }
  }
}


char inData[200];
int index = 0;
void serialEvent()
{
  int c = 0; 
  if (Serial.available() == 0) return;  // if no char to process skip
  
  c = Serial.read();
  inData[index++] = c;
  inData[index] = '\0';  // keep end of string right
  
  if (strstr(inData, "ON") != NULL) // http://www.cplusplus.com/reference/cstring/strstr/
  {
    DebugMode = true;
    index = 0;
  } 

  if (strstr(inData, "OFF") != NULL) // 2nd test
  {
    DebugMode = false;
    index = 0;
  } 

  if (index == 199)  // prevent buffer overflow
  {
    index = 0;
  }
  
  Serial.println("DebugMode changed");
}


