
/*
 * Created by Mo.Msaad 2020
 * Server node LoRa Gps Shield,reads data from Gps, 
 * then receives data from client loRa Shield. 
**/

#include <SoftwareSerial.h>
#include <TinyGPS.h>
#include <ArduinoJson.h>
#include <SPI.h>
#include <RH_RF95.h>
#define led 7

TinyGPS gps; //create object of class gps
RH_RF95 rf95;// same for radioHead
SoftwareSerial ss(4, 3);// TX , RX respectively 
String UUID = "369c397d-cd83-4ac0-a30c-6728c3704a31";
bool newData = false;
//<------------------------------------------------------>Methods<------------------------------------------------------>

void setup(){
  Serial.begin(115200);
  ss.begin(9600);
   pinMode(led,OUTPUT);
  Serial.print("GPS started..."); 
  Serial.println();
  delay(10);
    if (!rf95.init()){
    Serial.println("init failed");  
  }
    rf95.setFrequency(868.0);
}

void blinkLed(){
  digitalWrite(led,HIGH);
  delay(100);
  digitalWrite(led,LOW);
}

float communicate(){
   float b;
   if (rf95.available()){
    // Should be a message for us now   
    char buf[8];
    uint8_t len = sizeof(buf);
    
    if (rf95.recv(buf, &len)){
//      Serial.print("Temperature from client Node: ");
      float a = atof(buf);
      b  = a/100;
//      Serial.println(b);
      blinkLed();
    }
    else
    {
      Serial.println("recv failed");
    }
  }
  Serial.print("Temperature from client Node: ");
  Serial.println(b);
  return b;
}


// getting gps data.
void getGps(){
  for (unsigned long start = millis(); millis() - start < 1000;){// For one second we parse GPS data and report some key values
    while (ss.available()){
      char c = ss.read();
      if (gps.encode(c)) // Did a new valid sentence come in?
        newData = true;
    }
  }
  if (newData){
    //data initialization and extraction
    int year;
    byte month, day, hour, minute, second, hundredths;
    unsigned long age;
    float flat, flon, falt;
    char sz[32];
    gps.crack_datetime(&year, &month, &day, &hour, &minute, &second, &hundredths, &age);
    gps.f_get_position(&flat, &flon, &age);
    sprintf(sz, "%02d/%02d/%02d %02d:%02d:%02d ", month, day, year, hour, minute, second);

    //data modulation 
    DynamicJsonBuffer jBuffer;
    JsonObject& root = jBuffer.createObject();
    root["Device_id"] =   UUID;   
    root["date"] =   sz; 
    root["Latitude"] =   flat; 
    root["Longtitude"] =   flon; 
    root["Temperature"] = communicate();
    root.prettyPrintTo(Serial);//print or send later
    Serial.println();
  }
  else{
    Serial.println("** No characters received from GPS: check wiring, or wait until the GPS finds a signal **");
    }

}

void loop(){
  getGps();
  delay(5000);
}
