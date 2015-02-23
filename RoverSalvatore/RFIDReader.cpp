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


#include "Arduino.h"
#ifndef RFIDReader_h
#define RFIDReader_h
#include "RFIDReader.h"
#endif

#include <SPI.h>
#include <MFRC522.h>


RFIDReader::RFIDReader(int RSTPin, int SSPin)
{
  mfrc522 = new MFRC522(SSPin, RSTPin);	// Create MFRC522 instance
  
  SPI.begin();			// Init SPI bus
  mfrc522->PCD_Init();		// Init MFRC522
  ShowReaderDetails();	// Show details of PCD - MFRC522 Card Reader details
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
