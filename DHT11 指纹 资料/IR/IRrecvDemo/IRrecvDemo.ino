#include <IRremote.h> 

int RECV_PIN = 7; //使用数位脚位7接收红外线讯号
IRrecv irrecv(RECV_PIN); //初始化红外线讯号输入
decode_results results; //储存讯号的结构

void setup() 
{ 
  Serial.begin(9600); 
  // irrecv.blink13(true); //设为true的话，当收到讯号时，脚位13的LED便会闪烁
  irrecv.enableIRIn(); //启动接收
} 

void loop() { 
   // Serial.println("Waiting");
  if ( irrecv.decode(&results) ) { //接收红外线讯号并解码
    Serial.print("results value is "); //输出解码后的资料
    Serial.print(results.value, HEX);
    Serial.print(", bits is "); 
    Serial.print(results.bits); 
    Serial.print(", decode_type is "); 
    Serial.println(results.decode_type); 
    irrecv.resume(); //准备接收下一个讯号
  } 
} 
