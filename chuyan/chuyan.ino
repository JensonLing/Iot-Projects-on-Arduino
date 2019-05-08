#include <DYE_Fingerprint.h>
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);

DYE_Fingerprint finger = DYE_Fingerprint(&mySerial);
int c,p;
char a;
/**************************以上为指纹***********************/
#include "DHT.h"

#define DHTPIN 4     // what digital pin we're connected to

// Uncomment whatever type you're using!
#define DHTTYPE DHT11   // DHT 11
//#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor.
// Note that older versions of this library took an optional third parameter to
// tweak the timings for faster processors.  This parameter is no longer needed
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
/**************************以上为DHT11***********************/

void setup()  
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);
  finger.begin(57600);
  //Serial.println("\n\nfinger detect test");
  // set the data rate for the sensor serial port
  
  if (finger.verifyPassword()) {
    //Serial.println("Found fingerprint sensor!");
  } else {
    //Serial.println("Did not find fingerprint sensor :(");
    //while (1) { delay(1); }
  }
  //Serial.println("Waiting for valid finger...");
  dht.begin();
  pinMode(8, OUTPUT);
}

void loop()
{
  a=Serial.read();
  if(a == '1')
  {
    for(int i=0;i<30;i++)
    {
      if((c=ver())==1)
      {
        Serial.println(c);
        break;
      }
      
    }
    if(c==0) Serial.println(c);
  }
  
  else if(a == '2')
  {
    digitalWrite(8, HIGH);
    delay(100);
    float t = dht.readTemperature();
    Serial.println(t);
  }
  else if(a == '3')
  {
    digitalWrite(8, HIGH);
    delay(100);
    float h = dht.readHumidity();
    Serial.println(h);
  }
}

int ver()
{
   uint8_t p = finger.getImage();
   p = finger.image2Tz();
   p = finger.fingerFastSearch();
   if (p == FINGERPRINT_OK)  return 1;
   else return 0;
}

void verify()                     
{
  getFingerprintIDez();
  delay(50);            
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }
  
  // OK converted!
  p = finger.fingerFastSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }   
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence); 

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // found a match!
  Serial.print("Found ID #"); Serial.print(finger.fingerID); 
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
  return finger.fingerID; 
}
