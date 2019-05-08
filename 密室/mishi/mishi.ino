/* 
Code for 密室逃脱
author: 凌杰
E-mail: lingjie981119@126.com
Version 1.0   4/26/2015
*/
#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
byte a=0;
byte b=0;
byte c=0;
byte d=0;
void setup() {
lcd.begin(16, 2);
pinMode(6,INPUT);
pinMode(7,INPUT);
pinMode(8,INPUT);
pinMode(9,INPUT);
}

void loop() 
{
if(digitalRead(6)==HIGH){
a=a+1;
delay(150);
}
if(digitalRead(6)==LOW){
a=a;delay(150);}

if(digitalRead(7)==HIGH){
b=b+1;
}
if(digitalRead(7)==LOW){
b=b;}

if(digitalRead(8)==HIGH){
c=c+1;
}
if(digitalRead(8)==LOW){
c=c;}

if(digitalRead(9)==HIGH){
d=d+1;
}
if(digitalRead(9)==LOW){
d=d;}

lcd.setCursor(1, 0);
lcd.print("Enter The Code");
lcd.setCursor(4, 1);
lcd.print(a);
lcd.setCursor(6, 1);
lcd.print(b);
lcd.setCursor(8, 1);
lcd.print(c);
lcd.setCursor(10, 1);
lcd.print(d);

if(a==11&&b==10&&c==11&&d==11){
lcd.clear();
lcd.setCursor(2, 0);
lcd.print("Successfully");
lcd.setCursor(3, 1);
lcd.print("Identified!");
delay(1500);
lcd.clear();
lcd.setCursor(3, 0);
lcd.print("FATHER OF:");
lcd.setCursor(1, 1);
lcd.print("Direct Current");
delay(5000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
if(a>=12){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Fail To Identify");
lcd.setCursor(0, 1);
lcd.print("Please Try Again");
delay(3000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
if(b>=11){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Fail To Identify");
lcd.setCursor(0, 1);
lcd.print("Please Try Again");
delay(3000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
if(c>=12){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Fail To Identify");
lcd.setCursor(0, 1);
lcd.print("Please Try Again");
delay(3000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
if(d>=12){
lcd.clear();
lcd.setCursor(0, 0);
lcd.print("Fail To Identify");
lcd.setCursor(0, 1);
lcd.print("Please Try Again");
delay(3000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
if(a==2&&b==3&&c==4&&d==1){
lcd.clear();
lcd.setCursor(2, 0);
lcd.print("Successfully");
lcd.setCursor(3, 1);
lcd.print("Identified!");
delay(1500);
lcd.clear();
lcd.setCursor(7, 0);
lcd.print("EF");
lcd.setCursor(0, 1);
lcd.print("=>01010101010...");
delay(5000);
a=0;
b=0;
c=0;
d=0;
lcd.clear();
}
}
//Terrible coding style!!!
