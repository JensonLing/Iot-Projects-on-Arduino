/*
Zigbee centralized controller                                                              
Author: HuangSenhong
Email: senhonghuang@126.com
Time: 02/27/2016                                                               
*/
/*******************Port Interface**************
 Arduino Mega      2.8inch TFT Touch Shield 
  ICSP                    ICSP
  D3                      TP_IRQ
  D4                      TP_CS
  D5                      SD_CS
  D7                      LCD_DC
  D9                      LCD_BL
  D10                     LCD_CS  
/*******************Port Interface**************
 Arduino Mega          Zigbee DL-22 
  TX2                      RX
  RX2                      TX
  +3.3V                    VCC
  GND                      GND  
*/

#include <stdint.h>
#include <LCD.h>
#include <SPI.h>

byte Rx_buf[8] = {'0','0','0','0','0','0','0','0'};
byte Tem_buf[8] = {'T','0','0','0','0','0','0','0'};
byte Bath_buf[8] = {'B','0','0','0','0','0','0','0'};

byte Command_buf[2] = {'0','0'}; 
int index = 0;
int index02 = 0;
int flag = 0;
byte book01_state[2] = {'0','0'};
byte book02_state[2] = {'0','0'};
byte bathtub_state[3] = {'0','0','0'};
int waterFlag = 1;
int airFlag = 1;
int voiceFlag = 0;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial3.begin(9600);
  Serial.println("System Initiation");

  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();
  
  Tft.lcd_init();                                      // init TFT library
  Tft.lcd_display_string(60, 25, (const uint8_t *)"Humidity:", FONT_1608, RED); //一个字母是8个x单位
  Tft.lcd_display_string(36, 50, (const uint8_t *)"Temperature:", FONT_1608, RED);
  Tft.lcd_display_string(12, 75, (const uint8_t *)"AirConditioner:", FONT_1608, RED);
  Tft.lcd_display_string(0, 100, (const uint8_t *)"------------------------------", FONT_1608, BLACK);

  Tft.lcd_display_string(36, 125, (const uint8_t *)"BookCase 01:", FONT_1608, RED);
  Tft.lcd_display_string(36, 150, (const uint8_t *)"BookCase 02:", FONT_1608, RED);
  Tft.lcd_display_string(0, 175, (const uint8_t *)"------------------------------", FONT_1608, BLACK);  

  Tft.lcd_display_string(68, 200, (const uint8_t *)"Bathtub:", FONT_1608, RED);
}

void loop() { // run over and over

    Serial3.write(Tem_buf,8);
    delay(500);

    Serial3.write(Bath_buf,8);
    delay(500);

}

void serialEvent2() {
   while (Serial2.available()) {
    // get the new byte:
    
     Rx_buf[index]  = Serial2.read();
     Serial.print(Rx_buf[index]);
     if(Rx_buf[0] == 'T'|| Rx_buf[0] == 'B'|| Rx_buf[0] == 'V') index = index+1;
     else  index = 0;
     if(index > 7){
        index = 0;
        Serial.println();
        //Tft.lcd_fill_rect(0, 225,240,16,WHITE);
        if(Rx_buf[0] == 'T'){
            for(int i=0;i<8;i++) Tem_buf[i] = Rx_buf[i];
            Tft.lcd_fill_rect(140, 25,28,16,WHITE);        
            Tft.lcd_display_num(140, 25, Rx_buf[1]-48,1, FONT_1608, RED);
            Tft.lcd_display_num(148, 25, Rx_buf[2]-48,1, FONT_1608, RED);
            Tft.lcd_display_string(156, 25, (const uint8_t *)"%", FONT_1608, RED);
            Tft.lcd_fill_rect(140, 50,28,16,WHITE); 
            Tft.lcd_display_num(140, 50, Rx_buf[3]-48,1, FONT_1608, RED); 
            Tft.lcd_display_num(148, 50, Rx_buf[4]-48,1, FONT_1608, RED);
            Tft.lcd_display_string(156, 50, (const uint8_t *)"C", FONT_1608, RED);
            if(airFlag==1 && voiceFlag==0){
                 Tft.lcd_fill_rect(140, 75,50,16,WHITE);
                 if(Rx_buf[5]=='0' && Rx_buf[6]=='0') Tft.lcd_display_string(140, 75, (const uint8_t *)"OFF", FONT_1608, RED);
                 if(Rx_buf[5]=='0' && Rx_buf[6]=='1') Tft.lcd_display_string(140, 75, (const uint8_t *)"ONE", FONT_1608, RED);
                 if(Rx_buf[5]=='1' && Rx_buf[6]=='0') Tft.lcd_display_string(140, 75, (const uint8_t *)"TWO", FONT_1608, RED);
                 if(Rx_buf[5]=='1' && Rx_buf[6]=='1') Tft.lcd_display_string(140, 75, (const uint8_t *)"THREE", FONT_1608, RED);         
            }
          } 
        if(Rx_buf[0] == 'B'){
          //-------------------------------book 01----------------------------------------
          for(int i=0;i<8;i++) Bath_buf[i] = Rx_buf[i];
          if(book01_state[0]!=Rx_buf[1] || book01_state[1]!=Rx_buf[2]) Tft.lcd_fill_rect(140, 125,150,16,WHITE);
          book01_state[0] = Rx_buf[1];
          book01_state[1] = Rx_buf[2];
           if(Rx_buf[1] == '0' && Rx_buf[2] == '0'){
                 Tft.lcd_display_string(140, 125, (const uint8_t *)"NO books", FONT_1608, RED);
           }
           if(Rx_buf[1] == '1' && Rx_buf[2] == '0'){
                 Tft.lcd_display_string(140, 125, (const uint8_t *)"BOOK 02", FONT_1608, RED);
           }
           if(Rx_buf[1] == '0' && Rx_buf[2] == '1'){
                 Tft.lcd_display_string(140, 125, (const uint8_t *)"BOOK 01", FONT_1608, RED);
           }
           if(Rx_buf[1] == '1' && Rx_buf[2] == '1'){
                 Tft.lcd_display_string(140, 125, (const uint8_t *)"Unkown book", FONT_1608, RED);
           }           
            //-------------------------------book 02---------------------------------------- 
           if(book02_state[0]!=Rx_buf[3] || book02_state[1]!=Rx_buf[4]) Tft.lcd_fill_rect(140, 150,150,16,WHITE);
           book02_state[0] = Rx_buf[3];
           book02_state[1] = Rx_buf[4];
            
           if(Rx_buf[3] == '0' && Rx_buf[4] == '0'){
                 Tft.lcd_display_string(140, 150, (const uint8_t *)"NO books", FONT_1608, RED);
           }
           if(Rx_buf[3] == '1' && Rx_buf[4] == '0'){
                 Tft.lcd_display_string(140, 150, (const uint8_t *)"BOOK 02", FONT_1608, RED);
           }
           if(Rx_buf[3] == '0' && Rx_buf[4] == '1'){
                 Tft.lcd_display_string(140, 150, (const uint8_t *)"BOOK 01", FONT_1608, RED);
           }           
           if(Rx_buf[3] == '1' && Rx_buf[4] == '1'){
                 Tft.lcd_display_string(140, 150, (const uint8_t *)"Unkown book", FONT_1608, RED);
           }
            //-------------------------------bathtub----------------------------------------
           if(bathtub_state[0]!=Rx_buf[5] || bathtub_state[1]!=Rx_buf[6] || bathtub_state[2]!=Rx_buf[7]) Tft.lcd_fill_rect(140, 200,80,16,WHITE);
           bathtub_state[0] = Rx_buf[5];
           bathtub_state[1] = Rx_buf[6];
           bathtub_state[2] = Rx_buf[7];
            
           if(Rx_buf[5] == '1'&& waterFlag == 1)   Tft.lcd_display_string(140, 200, (const uint8_t *)"Unstable", FONT_1608, RED);
           if (Rx_buf[5] == '0'&& waterFlag == 1) {
                 if(Rx_buf[6] == '0' && Rx_buf[7] == '0'){
                       Tft.lcd_display_string(140, 200, (const uint8_t *)"EMPTY", FONT_1608, RED);
                 }
                 if(Rx_buf[6] == '0' && Rx_buf[7] == '1'){
                       Tft.lcd_display_string(140, 200, (const uint8_t *)"One third", FONT_1608, RED);
                 }
                 if(Rx_buf[6] == '1' && Rx_buf[7] == '0'){
                       Tft.lcd_display_string(140, 200, (const uint8_t *)"Two third", FONT_1608, RED);
                 }         
                 if(Rx_buf[6] == '1' && Rx_buf[7] == '1'){
                       Tft.lcd_display_string(140, 200, (const uint8_t *)"FULL", FONT_1608, RED);
                 }             
            }                
         
        }
       if(Rx_buf[0] == 'V'){
           if(Rx_buf[1] == 'K'){
                 Tft.lcd_fill_rect(0, 225,240,16,WHITE);
                 Tft.lcd_display_string(50, 225, (const uint8_t *)"Which mode(1,2,3)?", FONT_1608, RED);
                 voiceFlag = 1;
           }
           if(Rx_buf[1] == 'G'){
                 Tft.lcd_fill_rect(140, 75,50,16,WHITE);
                 Tft.lcd_fill_rect(0, 225,240,16,WHITE);
                 voiceFlag = 1;
                 Tft.lcd_display_string(140, 75, (const uint8_t *)"OFF", FONT_1608, RED);
                 Tem_buf[6] = '0';
                 Tem_buf[7] = '0';
                 Serial2.write('A');
                 Serial2.write('0');
           }
           if(Rx_buf[1] == '1'){
                 Tft.lcd_fill_rect(140, 75,50,16,WHITE);
                 Tft.lcd_fill_rect(0, 225,240,16,WHITE);
                 voiceFlag = 1;
                 Tft.lcd_display_string(140, 75, (const uint8_t *)"ONE", FONT_1608, RED);
                 Tem_buf[6] = '0';
                 Tem_buf[7] = '1';
                 Serial2.write('A');
                 Serial2.write('1');
           }
           if(Rx_buf[1] == '2'){
                 Tft.lcd_fill_rect(140, 75,50,16,WHITE);
                 Tft.lcd_fill_rect(0, 225,240,16,WHITE);
                 voiceFlag = 1;
                 Tft.lcd_display_string(140, 75, (const uint8_t *)"TWO", FONT_1608, RED);
                 Tem_buf[6] = '1';
                 Tem_buf[7] = '0';
                 Serial2.write('A');
                 Serial2.write('2');
           }
           if(Rx_buf[1] == '3'){
                 Tft.lcd_fill_rect(140, 75,50,16,WHITE);
                 Tft.lcd_fill_rect(0, 225,240,16,WHITE);
                 voiceFlag = 1;
                 Tft.lcd_display_string(140, 75, (const uint8_t *)"THREE", FONT_1608, RED);
                 Tem_buf[6] = '1';
                 Tem_buf[7] = '1';
                 Serial2.write('A');
                 Serial2.write('3');
           }                      
        }
       
      } 
  }
}


void serialEvent3() {
   while (Serial3.available()) {
    // get the new byte:
    
     Command_buf[index02]  = Serial3.read();
     Serial.print(Command_buf[index02]);
     if(Command_buf[0] == 'A'|| Command_buf[0] == 'W') index02 = index02+1;
     else  index02 = 0;
     if(index02 > 1){
        index02 = 0;
        Serial.println();
        if(Command_buf[0] == 'A'){
            Serial2.write(Command_buf[0]);
            Serial2.write(Command_buf[1]);
            airFlag = 0;
            Tft.lcd_fill_rect(140, 75,50,16,WHITE);
            switch(Command_buf[1]){
                case '0': Tft.lcd_display_string(140, 75, (const uint8_t *)"OFF", FONT_1608, RED); break;
                case '1': Tft.lcd_display_string(140, 75, (const uint8_t *)"ONE", FONT_1608, RED); break;
                case '2': Tft.lcd_display_string(140, 75, (const uint8_t *)"TWO", FONT_1608, RED); break;
                case '3': Tft.lcd_display_string(140, 75, (const uint8_t *)"THREE", FONT_1608, RED); break;
              }
         }

        if(Command_buf[0] == 'W'){
            Serial2.write(Command_buf[0]);
            Serial2.write(Command_buf[1]);
            waterFlag = 1;
            Tft.lcd_fill_rect(140, 200,150,16,WHITE);
            switch(Command_buf[1]){
                case '0': Tft.lcd_display_string(140, 200, (const uint8_t *)"EMPTY", FONT_1608, RED);break;
                case '1': Tft.lcd_display_string(140, 200, (const uint8_t *)"One third", FONT_1608, RED);break;
                case '2': Tft.lcd_display_string(140, 200, (const uint8_t *)"Two third", FONT_1608, RED);break;
                case '3': Tft.lcd_display_string(140, 200, (const uint8_t *)"FULL", FONT_1608, RED);break;
              }
        }

          }
   }
}


