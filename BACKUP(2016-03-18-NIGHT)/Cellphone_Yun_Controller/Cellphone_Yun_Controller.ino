/*
url:
请求：/arduino/temphum/1  温度和湿度
返回：T00000  第2和第3个bit表示适度，4、5个bit表示温度，6个bit表示档位(0、1、2、3共4档)

请求：/arduino/bookbath/1  书架和浴缸
返回：B0000  第2个bit表示书架1，3个bit表示书架2（0代表没有书，1代表有书1，2代表有书2,3代表无法识别的书）；
第4个bit（1表示水位读数未稳定；0表示读数稳定，此时才能读7、8bit）；第5个bit表示水位(0、1、2、3共4档)


请求：/arduino/air/0 手动控制空调档位(0、1、2、3共4档)
返回：A0

请求：/arduino/bathtub/0 手动控制浴缸水位(0、1、2、3共4档)
返回：B0
*/

#include <Bridge.h>
#include <BridgeServer.h>
#include <BridgeClient.h>

#include <SoftwareSerial.h>

SoftwareSerial mySerial(10, 11); // RX, TX

int index = 0;
byte Rx_buf[6] = {'0','0','0','0','0','0'};
byte TempHum[5] = {'0','0','0','0','0'};
byte BookBath[4] = {'0','0','0','0'};

// Listen to the default port 5555, the Yún webserver
// will forward there all the HTTP requests you send
BridgeServer server;

void setup() {
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  
   // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  
  // Bridge startup
  Bridge.begin();
  
  
  // Listen for incoming connection only from localhost
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();

  Serial.println("System Initiation");  
}

void loop() {
  // Get clients coming from server
  BridgeClient client = server.accept();

  // There is a new client?
  if (client) {
    // Process request
    process(client);

    // Close connection and free resources.
    client.stop();
  }

  mySerialEvent();
  delay(50); // Poll every 50ms
}

void process(BridgeClient client) {
  // read the command
  String command = client.readStringUntil('/');

  // is "temphum" command?
  if (command == "temphum") {
    temphumCommand(client);
  }

  // is "bookbath" command?
  if (command == "bookbath") {
    bookbathCommand(client);
  }

  // is "air" command?
  if (command == "air") {
    airCommand(client);
  }

  // is "bathtub" command?
  if (command == "bathtub") {
    bathtubCommand(client);
  }  
}

void temphumCommand(BridgeClient client) {
  int i = 0;
  char newChar[5];
  // Send feedback to client
  for(i=0;i<5;i++) newChar[i] = TempHum[i];

  for(i=0;i<4;i++)  client.print(newChar[i]);
  client.println(newChar[4]);
  
  Serial.println("Send TempHum Successfully!");

}


void bookbathCommand(BridgeClient client) {
  int j = 0;
  char newChar[5];
  for(j=0;j<4;j++) newChar[j] = BookBath[j];

  for(j=0;j<3;j++)  client.print(newChar[j]);
  client.println(newChar[3]);

  Serial.println("Send BookBath Successfully!");

}

void airCommand(BridgeClient client) {
  int airValue;

    // Read value and execute command
    airValue = client.parseInt();

    Serial.print("Power of Air conditioner:");
    Serial.println(airValue);

    mySerial.write('A');
    mySerial.write(IntToByte(airValue));
    // Send feedback to client
    //client.println(F("A1"));

}

void bathtubCommand(BridgeClient client) {
    int waterValue;

    // Read value and execute command
    waterValue = client.parseInt();

    Serial.print("Depth of water of the bathtub:");
    Serial.println(waterValue);

    mySerial.write('W');
    mySerial.write(IntToByte(waterValue));

    // Send feedback to client
    //client.println(F("B1"));

    

}

void mySerialEvent() {
   while (mySerial.available()) {
    // get the new byte:
       Rx_buf[index]  = mySerial.read();
       Serial.print(Rx_buf[index]);
       if(Rx_buf[0] == 'T'|| Rx_buf[0] == 'B') index = index+1;
       else  index = 0;
       if(index > 7){
          index = 0;
          Serial.println();
          if(Rx_buf[0] == 'T'){
              for(int i=0;i<4;i++)  TempHum[i] = Rx_buf[i+1];
              if(Rx_buf[5] == '0' && Rx_buf[6] == '0') TempHum[4] = '0';
              if(Rx_buf[5] == '0' && Rx_buf[6] == '1') TempHum[4] = '1';
              if(Rx_buf[5] == '1' && Rx_buf[6] == '0') TempHum[4] = '2';
              if(Rx_buf[5] == '1' && Rx_buf[6] == '1') TempHum[4] = '3';
          }
          if(Rx_buf[0] == 'B'){
              if(Rx_buf[1] == '0' && Rx_buf[2] == '0') BookBath[0] = '0';
              if(Rx_buf[1] == '0' && Rx_buf[2] == '1') BookBath[0] = '1';
              if(Rx_buf[1] == '1' && Rx_buf[2] == '0') BookBath[0] = '2';
              if(Rx_buf[1] == '1' && Rx_buf[2] == '1') BookBath[0] = '3';

              if(Rx_buf[3] == '0' && Rx_buf[4] == '0') BookBath[1] = '0';
              if(Rx_buf[3] == '0' && Rx_buf[4] == '1') BookBath[1] = '1';
              if(Rx_buf[3] == '1' && Rx_buf[4] == '0') BookBath[1] = '2';
              if(Rx_buf[3] == '1' && Rx_buf[4] == '1') BookBath[1] = '3';

              BookBath[2] = Rx_buf[5];
             
              if(Rx_buf[6] == '0' && Rx_buf[7] == '0') BookBath[3] = '0';
              if(Rx_buf[6] == '0' && Rx_buf[7] == '1') BookBath[3] = '1';
              if(Rx_buf[6] == '1' && Rx_buf[7] == '0') BookBath[3] = '2';
              if(Rx_buf[6] == '1' && Rx_buf[7] == '1') BookBath[3] = '3';              
          
          }          
    }
  }
}

byte IntToByte(int var){
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




