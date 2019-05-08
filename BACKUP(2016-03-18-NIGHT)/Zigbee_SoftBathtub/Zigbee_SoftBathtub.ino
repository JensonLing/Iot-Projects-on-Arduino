/*
Controller of Bathtub                                                            
Author: HuangSenhong
Email: senhonghuang@126.com
Time: 02/27/2016                                                               
*/
/*******************Port Interface**************
 Arduino Mega          Zigbee DL-22 
  TX3                      RX
  RX3                      TX
  +3.3V                    VCC
  GND                      GND
 /*******************Port Interface**************
 Arduino Mega          Arduino Nano 01 
  TX1                      D6
  RX1                      D8
  GND                      GND  
 /*******************Port Interface**************
 Arduino Mega          Arduino Nano 02 
  TX2                      D6
  RX2                      D8
  GND                      GND  
/*******************Port Interface**************
 Arduino Mega           LCD 1602 
  GND                    Pin1 VSS
  5V                     Pin2 VCC
  1K+GND                 Pin3 VL
  D12                    Pin4 RS
  GND                    Pin5 R/W
  D11                    Pin6 Enable  

  D5                     Pin11 D4
  D4                     Pin12 D5
  D3                     Pin13 D6
  D2                     Pin14 D7
  3.3V                   Pin15 BLA
  GND                    Pin16 BLK

*/

#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int index = 0;
int index01 = 0;
int index02 = 0;
int index03 = 0;

byte Bookcase01_buf[4] = {'0','0','0','0'};
byte Bookcase02_buf[4] = {'0','0','0','0'};
byte Tx_buf[8] = {'B','0','0','0','0','0','0','0'};
byte Water_buf[4] = {'0','0','0','0'};

byte Command_buf[2] = {'0','0'}; 
String bookState[4] = {"No","B1","B2","Un"};
int bookCase01 = 0;
int pre_bookCase01 = 0;
int bookCase02 = 0;
int pre_bookCase02 = 0;
String waterLevel[5] = {"Empty","1/3","2/3","Full","Unstable"};
int waterDepth = 0;
int pre_waterDepth = 0;
int waterFlag = 1;

// SoftwareSerial mySerial(10, 11); // RX, TX
SoftwareSerial mySerial(10, 6); // RX, TX

void setup()
{
    Serial.begin(9600);
    Serial1.begin(9600); 
    Serial2.begin(9600); 
    Serial3.begin(9600); 
    mySerial.begin(9600);
    lcd.begin(16, 2);
    lcd.setCursor(0, 0);
    lcd.print("BC1:No");
    lcd.setCursor(8, 0);
    lcd.print("BC2:No");
    lcd.setCursor(0, 1);
    lcd.print("Water: Empty");
    pinMode(7,OUTPUT);
    digitalWrite(7,LOW);
}

void loop()
{
    mySerialEvent();
    
    Tx_buf[1] = Bookcase01_buf[2];
    Tx_buf[2] = Bookcase01_buf[3];
    Tx_buf[3] = Bookcase02_buf[2];
    Tx_buf[4] = Bookcase02_buf[3];  
    for (int i = 0;i<8;i++){
      Serial3.write(Tx_buf[i]);
    }
    delay(1000);
}

void mySerialEvent() {
   while (mySerial.available()) {
    // get the new byte:
       Water_buf[index]  = mySerial.read();
       Serial.print(Water_buf[index]);
      index = index+1;
      if(index >= 4){
        index = 0;
        Serial.println();
        Tx_buf[5] = Water_buf[1];
        Tx_buf[6] = Water_buf[2];
        Tx_buf[7] = Water_buf[3];
        if(Water_buf[1] == '1') waterDepth = 4;
        if(Water_buf[1] == '0' && waterFlag){
            if(Water_buf[2] == '0' && Water_buf[3] == '0' ) waterDepth = 0;
            if(Water_buf[2] == '0' && Water_buf[3] == '1' ) waterDepth = 1;
            if(Water_buf[2] == '1' && Water_buf[3] == '0' ) waterDepth = 2;
            if(Water_buf[2] == '1' && Water_buf[3] == '1' ) waterDepth = 3;
          }
         if(pre_waterDepth!=waterDepth) LCD_show();
         pre_waterDepth = waterDepth;     
      }    
  }
}

void serialEvent1() {
  while (Serial1.available()) {
    // get the new byte:
    Bookcase01_buf[index01]  = Serial1.read();
//     Serial.print(Bookcase01_buf[index01]);
     if(Bookcase01_buf[0] == 'B') index01 = index01 + 1;
     else  index01 = 0;
     if(index01 > 4){
        index01 = 0;
//        Serial.println();
        if(Bookcase01_buf[2] == '1' && Bookcase01_buf[3] == '1'){
              // Serial.println("I don't know what this book is!");
              bookCase01 = 3;    
          } 
        if(Bookcase01_buf[2] == '1' && Bookcase01_buf[3] == '0'){
              // Serial.println("Book02 is IN the bookcase!");
              bookCase01 = 2;      
          }
        if(Bookcase01_buf[2] == '0' && Bookcase01_buf[3] == '1'){
               //Serial.println("Book01 is IN the bookcase!"); 
              bookCase01 = 1;     
          }           
        if(Bookcase01_buf[2] == '0' && Bookcase01_buf[3] == '0'){
              // Serial.println("Book01 or 02 is OUT of the bookcase!");
              bookCase01 = 0;     
          } 

        if(pre_bookCase01!=bookCase01) LCD_show();
        pre_bookCase01 = bookCase01;
  }
}
}

void serialEvent2() {
  while (Serial2.available()) {
    // get the new byte:
    Bookcase02_buf[index02]  = Serial2.read();
//     Serial.print(Bookcase02_buf[index02]);
     if(Bookcase02_buf[0] == 'B') index02 = index02 + 1;
     else  index02 = 0;
     if(index02 > 4){
        index02 = 0;
//        Serial.println();
        if(Bookcase02_buf[2] == '1' && Bookcase02_buf[3] == '1'){
               //Serial.println("I don't know what this book is!");
               bookCase02 = 3;      
          } 
        if(Bookcase02_buf[2] == '1' && Bookcase02_buf[3] == '0'){
               //Serial.println("Book02 is IN the bookcase!");
               bookCase02 = 2;      
          }
        if(Bookcase02_buf[2] == '0' && Bookcase02_buf[3] == '1'){
              // Serial.println("Book01 is IN the bookcase!");
              bookCase02 = 1;     
          }           
        if(Bookcase02_buf[2] == '0' && Bookcase02_buf[3] == '0'){
              // Serial.println("Book01 or 02 is OUT of the bookcase!"); 
              bookCase02 = 0;     
          }

        if(pre_bookCase02!=bookCase02) LCD_show();
        pre_bookCase02 = bookCase02;
  }   
  }
}

void serialEvent3() {
  while (Serial3.available()) {
    // get the new byte:
    Command_buf[index03] = Serial3.read();
    Serial.print(Command_buf[index03]);
    if(Command_buf[0] == 'W') index03 = index03+1;
     else  index03 = 0;
     if(index03 > 1){
        index03 = 0;
        digitalWrite(7,HIGH);
        Serial.println();
        waterDepth = Command_buf[1] -48;
        waterFlag = 0;
     }
  }
}

void LCD_show(){
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("BC1:");
    lcd.setCursor(4, 0);
    lcd.print(bookState[bookCase01]);    
    lcd.setCursor(8, 0);
    lcd.print("BC2:");
    lcd.setCursor(12, 0);
    lcd.print(bookState[bookCase02]);     
    lcd.setCursor(0, 1);
    lcd.print("Water:");  
    lcd.setCursor(7, 1);
    lcd.print(waterLevel[waterDepth]);   
  
  }


