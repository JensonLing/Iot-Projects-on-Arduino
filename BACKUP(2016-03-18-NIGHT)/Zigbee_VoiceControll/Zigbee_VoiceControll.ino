/*******************Port Interface**************
 Arduino Uno             LD 3320 
  3.3V                     VCC         
  GND                      GND 
  D12                      MISO
  D11                      MOSI
  D13                      SCK
  D4                       NSS
  D9                       RSI
  D2                       IRQ
  GND                      WR
/*******************Port Interface**************
 Arduino Uno          Zigbee DL-22 
  D6                       RX
  D8                       TX
  +3.3V                    VCC
  GND                      GND
*/


#include <ld3320.h>
#include <SoftwareSerial.h>

VoiceRecognition Voice;                         //声明一个语音识别对象

SoftwareSerial mySerial(8, 6); // RX, TX
byte Tx_buf[8] = {'V','0','0','0','0','0','0','0'};

void setup() 
{
    Serial.begin(9600);
    mySerial.begin(9600);
    
    Voice.init();                               //初始化VoiceRecognition模块   
    Voice.addCommand("kai kong tiao",0);             //添加指令，参数（指令内容，指令标签（可重复））
    Voice.addCommand("guan kong tiao",1);            //添加指令，参数（指令内容，指令标签（可重复））
    Voice.addCommand("mo shi yi",2);
    //Voice.addCommand("mo si yi",3);
    Voice.addCommand("mo shi er",4);
    //Voice.addCommand("mo si er",5);
    Voice.addCommand("mo shi san",6);
    //Voice.addCommand("mo si san",7);    
    
    Voice.start();//开始识别
    Serial.println("System start!");
}
void loop() {
  switch(Voice.read())                          //判断识别
  {
    case 0:                                     
        Serial.println("kai kong tiao");
        Tx_buf[1] = 'K';
        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
        break;
    case 1:                                    
        Serial.println("guan kong tiao");
        Tx_buf[1] = 'G';
        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
        break;   
    case 2:                                    
        Serial.println("mo shi yi");
        Tx_buf[1] = '1';
        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
        break; 
//    case 3:                                     
//        Serial.println("mo si yi");
//        Tx_buf[1] = '1';
//        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
//        break; 
    case 4:                                     
        Serial.println("mo shi er");
        Tx_buf[1] = '2';
        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
        break;
//    case 5:                                     
//        Serial.println("mo si er");
//        Tx_buf[1] = '2';
//        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
//        break;
    case 6:                                     
        Serial.println("mo shi san");
        Tx_buf[1] = '3';
        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
        break;
//    case 7:                                     
//        Serial.println("mo si san");
//        Tx_buf[1] = '3';
//        for(int i=0;i<8;i++) mySerial.write(Tx_buf[i]);
//        break;
   default:
      //Serial.println("shit");
        break;
  }
}
