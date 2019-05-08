/* 
Code for 密室逃脱P2
author: 凌杰
E-mail: lingjie981119@126.com
Version 1.0   4/27/2015
*/
#include <Servo.h>
Servo myservo; 
int pos = 180;
byte m=0;
byte s=0;
void setup()
{
 myservo.attach(5);
 pinMode(2,OUTPUT);
 pinMode(3,OUTPUT);
 Serial.begin(9600);
 Serial1.begin(9600);
}
 
void loop()
{
  if(m>=1){
  s=s+1;
  delay(1000);}
  if(s==60){s=0;m=m+1;}
  if(s>=1&&m==0){
  s=s+1;
  delay(1000);}
  if(s==60){s=0;m=m+1;}
  while(Serial1.available())
   {
     char c=Serial1.read();
      if(c=='S')
        { s=1;m=0;
          Serial1.println("Welcome! Enjoy Your Time Here!"); 
      }
      if(c=='@')
      {Serial1.println("Game Over,Welcome Back Next Time!");
      Serial1.print("You've Spent ");
      Serial1.print(m);
      Serial1.print(" minutes ");
      Serial1.print(s);
      Serial1.println(" seconds in all!");
      Serial1.print("Then,why not go to CLASS 12 and HAVE FUN(:3)");
      for(int i = pos; i >= 0; i -= 1)  // goes from 0 degrees to 180 degrees 
      {                                  // in steps of 1 degree 
       myservo.write(i);              // tell servo to go to position in variable 'pos' 
       delay(10);                       // waits 15ms for the servo to reach the position 
       }
       pos=0;
       digitalWrite(3,HIGH);
       digitalWrite(2,LOW);               
       delay(10000);
       
       for(int i = pos; i <= 180; i += 1)  // goes from 0 degrees to 180 degrees 
       {                                  // in steps of 1 degree 
        myservo.write(i);              // tell servo to go to position in variable 'pos' 
        delay(10);                       // waits 15ms for the servo to reach the position 
        }
       pos=180;
    }
      if(c=='b')
      {
       Serial1.println("Listen to the sound carefully and find the Code");
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(500);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(500);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(100);
       tone(7, 650);
       delay(200);
       noTone(7);
       delay(500);
       tone(7, 650);
       delay(200);
       noTone(7);
    }
   }
    if(m==1&&s==1){
    Serial1.println("You've been in the room for 1 minute.");
    Serial1.println("Tip1:Have you noticed the red LED? There's something SPECIAL there.");
    }
    if(m==3&&s==1){
    Serial1.println("You've been in the room for 3 minute.");
    Serial1.println("Tip2:Turn the two letters you've got into 100100111011......");
    }
    if(m==5&&s==1){
    Serial1.println("You've been in the room for 5 minute.");
    Serial1.print("Tip3:Direct Current means");
    Serial1.println("'zhi liu dian' in Chinese.");
    }
    if(m==6&&s==1){
    Serial1.println("You've been in the room for 6 minute.");
    Serial1.println("OK...It seems that you've tried hard but failed to find the clue.");
    Serial1.println("Get the Pad behind the TV.Type in 'Edison' and you will know all.");
    }
   if(pos!=0){digitalWrite(2,HIGH);
   digitalWrite(3,LOW);}
   Serial.println(s);
   
}
