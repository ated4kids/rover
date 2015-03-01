//#include <Wire.h>
// Reference the HMC5883L Compass Library
#include <HMC5883L.h>

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

/*
VCC su 5v
GND su ground
SCL su SCL
SDA su SDA
*/

class Compass
{
public:
  Compass();
  float ReadPosition();
private:
  // Store our compass as a variable.
  HMC5883L compass;
  // Record any errors that may occur in the compass.
  int error = 0;
};


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

struct RGB
{
public:
  int R;
  int G;
  int B;
};


class ColorSensor
{
public:
  RGB GetRGB();
  ColorSensor(int vcc, int out, int s2, int s3, int s0, int s1);
private:
  int red;
  int grn;
  int blu;

  int VCC;
  int OUT;
  int S2;
  int S3;
  int S0;
  int S1;
  
  int RGBFilter;
  long count;
  int counter;
  int G_flag ;
  float scaleFactor[3];

  void Select_Filters(int RGB);  
};




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


class RFIDReader
{
public:
  long GetUID();
  RFIDReader(int RSTPin, int SSPin);
  void ShowReaderDetails();
  void ShowCardID();
  void DumpCard();
private:
  MFRC522* mfrc522;
};


/*
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
ULTRASONIC SENSOR
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
****************************************************************************
*/

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



