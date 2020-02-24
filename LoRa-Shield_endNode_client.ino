
/*Simple client sketch from RadioHead Library.
 * modified by Mo.Msaad 20/02/2020
 * Client end node measures temperature and sends packet via RH frequency 868
 * to change frequency locate /Documents/Arduino/Libraries/RadioHead/RH_RF95 and find 
 * method setFrequency() and set it to desired frequ according to your board specification
*/

#include <SPI.h>
#include <RH_RF95.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#define DHTPIN 5 
#define DHTTYPE    DHT11 //define sensor type, like DHt22 ...etc.
DHT_Unified dht(DHTPIN, DHTTYPE);

// Singleton instance of the radio driver
RH_RF95 rf95;

float temp;
int count = 0;

void setup(){
  Serial.begin(9600); // Start arduino serial
  dht.begin();   // Start DHT sensor
  while (!Serial) ; // Wait for serial port to be available
  if (!rf95.init()){
    Serial.println("init failed");
  }
   rf95.setTxPower(13, true);
}

float getTemperature(){
  float tempo;
  sensors_event_t event;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  }
  else {
    tempo = event.temperature ;
    Serial.print("Temperature: ");
    Serial.print(tempo);
    Serial.print(" °C");
    Serial.println();
  }
  return tempo;
}

// Send a temperature data to rf95_LoRaGPS
void sendDataToGPS(){
  Serial.println("Sending from LoRa-Shield to LoRa-GPS>>...");
     int a  = getTemperature()*100;
     uint8_t data = a;
     rf95.send(a, sizeof(4));
     rf95.waitPacketSent();  // Now wait for a reply
     Serial.println("Packet sent");
    }

void loop(){
  sendDataToGPS();
  delay(5000);
}
