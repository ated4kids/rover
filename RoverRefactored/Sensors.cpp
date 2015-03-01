#include "Arduino.h"

#ifndef Sensors_h
  #define Sensors_h
  #include "Sensors.h"
#endif

#include <SPI.h>
#include <MFRC522.h>


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
COMPASS
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/


Compass::Compass(bool debugging)
{
  m_debug = debugging;
  
  if (m_debug)
     Serial.println("Starting the I2C interface.");
  Wire.begin(); // Start the I2C interface.

  if (m_debug)
     Serial.println("Constructing new HMC5883L");
  compass = HMC5883L(); // Construct a new HMC5883 compass.
    
  if (m_debug)
     Serial.println("Setting scale to +/- 1.3 Ga");
  error = compass.SetScale(1.3); // Set the scale of the compass.
  if(error != 0) // If there is an error, print it out.
    if (m_debug)
       Serial.println(compass.GetErrorText(error));
  
  if (m_debug)
     Serial.println("Setting measurement mode to continous.");
  error = compass.SetMeasurementMode(Measurement_Continuous); // Set the measurement mode to Continuous
  if(error != 0) // If there is an error, print it out.
    if (m_debug)
       Serial.println(compass.GetErrorText(error));}


float Compass::ReadPosition()
{
 // Retrive the raw values from the compass (not scaled).
  MagnetometerRaw raw = compass.ReadRawAxis();
  // Retrived the scaled values from the compass (scaled to the configured scale).
  MagnetometerScaled scaled = compass.ReadScaledAxis();
  
  // Values are accessed like so:
  int MilliGauss_OnThe_XAxis = scaled.XAxis;// (or YAxis, or ZAxis)

  // Calculate heading when the magnetometer is level, then correct for signs of axis.
  float heading = atan2(scaled.YAxis, scaled.XAxis);
  
  // Once you have your heading, you must then add your 'Declination Angle', which is the 'Error' of the magnetic field in your location.
  // Find yours here: http://www.magnetic-declination.com/
  // Mine is: 2? 37' W, which is 2.617 Degrees, or (which we need) 0.0456752665 radians, I will use 0.0457
  // If you cannot find your Declination, comment out these two lines, your compass will be slightly off.
  float declinationAngle = 0.0215;
  heading += declinationAngle;
  
  // Correct for when signs are reversed.
  if(heading < 0)
    heading += 2*PI;
    
  // Check for wrap due to addition of declination.
  if(heading > 2*PI)
    heading -= 2*PI;
   
  // Convert radians to degrees for readability.
  float headingDegrees = heading * 180/M_PI; 

  // Output the data via the serial port.
  return headingDegrees;

  // Normally we would delay the application by 66ms to allow the loop
  // to run at 15Hz (default bandwidth for the HMC5883L).
  // However since we have a long serial out (104ms at 9600) we will let
  // it run at its natural speed.
  // delay(66);
}



/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
COLOR SENSOR
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/

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
    
    if (i == 0)
    {
      ret.R = scaleFactor[i]*count;
    }
    else if (i == 1)
    {
      ret.G = scaleFactor[i]*count;
    }
    else if (i == 2)
    {
      ret.B = scaleFactor[i]*count;
    }
    else 
    { 
      if (m_debug)
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

ColorSensor::ColorSensor(bool debug, int vcc, int out, int s2, int s3, int s0, int s1)
{
  m_debug = debug;

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



/*
 * ----------------------------------------------------------------------------
 * This is a MFRC522 library example; see https://github.com/miguelbalboa/rfid
 * for further details and other examples.
 * 
 * NOTE: The library file MFRC522.h has a lot of useful info. Please read it.
 * 
 * Released into the public domain.
 * ----------------------------------------------------------------------------
 * Example sketch/program showing how to read data from a PICC (that is: a RFID
 * Tag or Card) using a MFRC522 based RFID Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout
 * below), load this sketch into Arduino IDE then verify/compile and upload it.
 * To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M).
 * When you present a PICC (that is: a RFID Tag or Card) at reading distance
 * of the MFRC522 Reader/PCD, the serial output will show the ID/UID, type and
 * any data blocks it can read. Note: you may see "Timeout in communication"
 * messages when removing the PICC from reading distance too early.
 * 
 * If your reader supports it, this sketch/program will read all the PICCs
 * presented (that is: multiple tag reading). So if you stack two or more
 * PICCs on top of each other and present them to the reader, it will first
 * output all details of the first and then the next PICC. Note that this
 * may take some time as all data blocks are dumped, so keep the PICCs at
 * reading distance until complete.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */


RFIDReader::RFIDReader(bool debug, int RSTPin, int SSPin)
{
  m_debug = debug;
  
  mfrc522 = new MFRC522(SSPin, RSTPin);	// Create MFRC522 instance
  
  SPI.begin();			// Init SPI bus
  mfrc522->PCD_Init();		// Init MFRC522
  ShowReaderDetails();	// Show details of PCD - MFRC522 Card Reader details
  if (m_debug)
     Serial.println("Scan PICC to see UID, type, and data blocks...");
}

long RFIDReader::GetUID()
{
  // Look for new cards
  if ( ! mfrc522->PICC_IsNewCardPresent()) 
  {
    return 0;
  }
  
  // Select one of the cards
  if ( ! mfrc522->PICC_ReadCardSerial()) 
  {
    return 0;
  }
  
  //Transform Card id in long
  long ret = 0;
  for (byte i = 0; i < mfrc522->uid.size; i++) 
  {
    ret += mfrc522->uid.uidByte[i];
  }   
  
  if (ret != 0)
    if (m_debug)
       Serial.println(ret);
  
  return ret;
}



void RFIDReader::DumpCard() 
{
  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522->PICC_DumpToSerial(&(mfrc522->uid));
}

void RFIDReader::ShowCardID() 
{
  //Print Card ID
  for (byte i = 0; i < mfrc522->uid.size; i++) {
    Serial.print(mfrc522->uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522->uid.uidByte[i], HEX);
  } 
  Serial.println();
}

void RFIDReader::ShowReaderDetails() {
  // Get the MFRC522 software version
  byte v = mfrc522->PCD_ReadRegister(mfrc522->VersionReg);
  Serial.print("MFRC522 Software Version: 0x");
  Serial.print(v, HEX);
  if (v == 0x91)
    Serial.print(" = v1.0");
  else if (v == 0x92)
    Serial.print(" = v2.0");
  else
    Serial.print(" (unknown)");
  
  Serial.println("");
  // When 0x00 or 0xFF is returned, communication probably failed
  if ((v == 0x00) || (v == 0xFF)) {
    Serial.println("WARNING: Communication failure, is the MFRC522 properly connected?");
  }
}



/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
COLOR SENSOR
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/


UltraSonicSensor::UltraSonicSensor (bool debug, int pingpin, int echopin)
{
  m_debug = debug;
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



