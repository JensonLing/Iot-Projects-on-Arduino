/*
Controller of Bookcase                                                            
Author: Jerison Ling
Email: lingjie981119@126.com
Time: 05/09/2019                                                               
*/
/*******************Port Interface**************
 Arduino Nano          Zigbee DL-22 
  TX2                      RX
  RX2                      TX
  +3.3V                    VCC
  GND                      GND  
/*******************Port Interface**************
/* -----------------------------------------------------------------------------------------
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

#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h>
SoftwareSerial mySerial(8, 6); // RX, TX
byte Tx_buf[4] = {'B','2','0','0'};

#define SS_PIN 10
#define RST_PIN 9
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte bookID01[4] = {0x24, 0x15, 0x98, 0xFC};
byte bookID02[4] = {0x89, 0x6B, 0x9C, 0x1A};
int isIn = 0;

void setup() { 
  Serial.begin(9600);
  mySerial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
}
 
void loop() {

  // Look for new cards
 if( !rfid.PICC_IsNewCardPresent());

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial()){
    isIn = isIn + 1;
    if(isIn >= 2){
       isIn = 0;
       Serial.println("Book 01 or 02 is OUT of the bookcase!");
       Tx_buf[2] = '0';
       Tx_buf[3] = '0';
       for(int i=0;i<4;i++)  mySerial.write(Tx_buf[i]);
    }
    delay(500);
    return;}
   else isIn = 0;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

 if(CompareBookID(bookID01)){
    Serial.println("Book 01 is IN the bookcase!");
    Tx_buf[2] = '0';
    Tx_buf[3] = '1';
    for(int i=0;i<4;i++)  mySerial.write(Tx_buf[i]);
 }
  else if(CompareBookID(bookID02)){
    Serial.println("Book 02 is IN the bookcase!");
    Tx_buf[2] = '1';
    Tx_buf[3] = '0';
    for(int i=0;i<4;i++)  mySerial.write(Tx_buf[i]);
 }  
 else{
    Serial.println("I don't know what this book is");
    Tx_buf[2] = '1';
    Tx_buf[3] = '1';
    for(int i=0;i<4;i++)  mySerial.write(Tx_buf[i]);  
  }

  delay(500);

  // Halt PICC
  //rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

int CompareBookID(byte arrayID[]) {
    int i = 0;
    int flag = 1;
   for(i=0;i<4;++i)
   {
      if(rfid.uid.uidByte[i]!=arrayID[i]) {flag=0;break;}
   }
   return flag; 
}

void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

