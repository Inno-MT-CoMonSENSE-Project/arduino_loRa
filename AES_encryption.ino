
#include <AESLib.h>
#include <Base64.h>
#include <TH02_dev.h> //here you need to include your DHT library instead
#include "Arduino.h"
#include "Wire.h" 

const char PROGMEM key[16] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15};//16bytes key,

void setup(){
 Serial.begin(9600);
  delay(150);/* Power up,delay 150ms,until voltage is stable */
  TH02.begin();
  delay(100);
  digitalWrite(SDA,LOW);
  digitalWrite(SCL,LOW);
   Serial.println("TH02_dev is available.\n");
}

String getTemperature(){
    int a = TH02.ReadTemperature()*100;
    String s = (String) a;
    Serial.print("Temperature: ");
    Serial.print(s);
    Serial.print(" °C");
    Serial.println();
    
  return s;
}

void loop(){
  encrypte();
  delay(3000);
}

void encrypte(){                                        


 String dataString = getTemperature();// get data
 char molde[dataString.length()+1]; 
 dataString.toCharArray(molde, dataString.length()+1);// convert to char array

 Serial.print("Raw Data: ");
 Serial.print(molde);
 Serial.println();
 Serial.print("Data size: ");
 Serial.print(sizeof(molde));
 Serial.println();

//---------------------------encode data-------------------------  
 int dataLenght = sizeof(molde);
 int encodedLength = Base64.encodedLength(dataLenght);
 char encodedString[encodedLength];
 Base64.encode(encodedString, molde, dataLenght);
 Serial.print("Encoded data is:" );
 Serial.print(encodedString);
 Serial.println();


////--------------------------encrypt data ------------------------
 aes256_enc_single(encodedString, key);
 Serial.print("encrypted data: ");
 for (int i= 0; i<sizeof(encodedString); i++) { //printing in HEX format otherwise you get unreadable chars
   Serial.print(encodedString[i]&0xFF, HEX);
   }
   Serial.println();

//--------------------------decrypt data ------------------------
   aes128_dec_single(encodedString, key);
   Serial.print("Decrypted data: ");
   Serial.print(encodedString);
   Serial.println();

//--------------------------decode data ------------------------
  int dataLenght1 = sizeof(encodedString);
  int decodedLength = Base64.decodedLength(encodedString,dataLenght1);
  char decodedString[decodedLength];
  Base64.decode(decodedString, encodedString, dataLenght1);
  int a =  atoi( decodedString );
  float b = a/100.00;   //dividing with 100.00 to not lose decimal precision
  Serial.print("Decoded string is: ");
  Serial.print(b);
  Serial.println();  

}
 
