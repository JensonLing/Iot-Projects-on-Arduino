 /*
Controller of AirCondition                                                            
Author: HuangSenhong
Email: senhonghuang@126.com
Time: 02/27/2016                                                               
*/
/*******************Port Interface**************
 Arduino Mega            DHT-11 
  D20                      DAT
  +5V                      VCC
  GND                      GND 
/*******************Port Interface**************
 Arduino Mega          Zigbee DL-22 
  TX2                      RX
  RX2                      TX
  +3.3V                    VCC
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



/*
  Board            int.0   int.1   int.2   int.3   int.4   int.5
 Uno, Ethernet    2   3
 Mega2560   2   3   21    20    19    18
 Leonardo   3   2   0   1
 Due            (any pin, more info http://arduino.cc/en/Reference/AttachInterrupt)
 
 This example, as difference to the other, make use of the new method acquireAndWait()
 */

 #include <idDHT11.h>
 #include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

 
byte Tx_buf[8] = {'T','0','0','0','0','0','0','0'};
byte Command_buf[2] = {'0','0'}; 
int index = 0;

int humidity = 0;
int temperature = 0;
int pre_humidity = 0;
int pre_temperature = 0;
int airPower = 0;
int pre_airPower = 0;
int flagPower = 1;
int idDHT11pin = 20; //Digital pin for comunications
int idDHT11intNumber = 3; //interrupt number (must be the one that use the previus defined pin (see table above)

//declaration
void dht11_wrapper(); // must be declared before the lib initialization

// Lib instantiate
idDHT11 DHT11(idDHT11pin,idDHT11intNumber,dht11_wrapper);

void setup()
{
  Serial.begin(9600);
  Serial2.begin(9600);
  Serial.println("System Init");
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Tem:00C");
  lcd.setCursor(8, 0);
  lcd.print("Hum:00%");
  lcd.setCursor(0, 1);
  lcd.print("Air: OFF");  
}
// This wrapper is in charge of calling 
// mus be defined like this for the lib work
void dht11_wrapper() {
  DHT11.isrCallback();
}
void loop()
{

  
  int result = DHT11.acquireAndWait();
  humidity = (int)DHT11.getHumidity();
  temperature = (int)DHT11.getCelsius();
  airCondition();
//   Serial.println(DHT11.getHumidity());
//   Serial.println(DHT11.getHumidity(),2);
   Serial.println(humidity);
   Serial.println(temperature);
  Tx_buf[1] = IntToByte(humidity/10);
  Tx_buf[2] = IntToByte(humidity%10);
  Tx_buf[3] = IntToByte(temperature/10);
  Tx_buf[4] = IntToByte(temperature%10);
  if(humidity!=pre_humidity || temperature!=pre_temperature || airPower!=pre_airPower) LCD_show();
  pre_humidity = humidity;
  pre_temperature = temperature;
  pre_airPower = airPower;

  for (int i = 0;i<8;i++){
    Serial2.write(Tx_buf[i]);
  }
  Serial.println();
  delay(1000);
}

byte IntToByte(unsigned int var){
   byte value;
   switch(var){
      case 0: 
          value = '0'; break;
      case 1: 
          value = '1'; break;
      case 2:
          value = '2'; break;
      case 3:
          value = '3'; break;
      case 4:
          value = '4'; break;
      case 5:
          value = '5'; break;
      case 6:
          value = '6'; break;
      case 7:
          value = '7'; break;
      case 8:
          value = '8'; break;
      case 9:
          value = '9'; break;
      default:
          value = '0'; break;         
   }
   return value;
}

void serialEvent2() {
  while (Serial2.available()) {
    // get the new byte:
    Command_buf[index] = Serial2.read();
    Serial.print(Command_buf[index]);
    if(Command_buf[0] == 'A') index = index+1;
     else  index = 0;
     if(index > 1){
        index = 0;
        Serial.println();
        airPower = Command_buf[1] -48;
        flagPower = 0;
     }
  }
}

void LCD_show(){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Tem:  C");
   lcd.setCursor(4, 0);
   lcd.print(temperature);       
   lcd.setCursor(8, 0);
   lcd.print("Hum:  %");
   lcd.setCursor(12, 0);
   lcd.print(humidity);
   lcd.setCursor(0, 1);
   lcd.print("Air: ");
   lcd.setCursor(5, 1);
   if(airPower == 0) lcd.print("OFF");
   else lcd.print(airPower);        
  
}

void airCondition(){
   if(flagPower){
       if(temperature>25) {
           airPower = 3;
           Tx_buf[5] = '1';
           Tx_buf[6] = '1';
       }
       else if (temperature>20){ 
          airPower = 2;
          Tx_buf[5] = '1';
          Tx_buf[6] = '0';
       }
       else if (temperature>16) {
          airPower = 1;
          Tx_buf[5] = '0';
          Tx_buf[6] = '1';
       }
       else{
          airPower = 0;
          Tx_buf[5] = '0';
          Tx_buf[6] = '0';
       }
   }
}



