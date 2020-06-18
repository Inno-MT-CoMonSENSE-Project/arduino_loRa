# Arduino based end-node sending data over LoRa to LoRaWAN network
This sketch reads sensor's data, encode them to Cayenne LPP format and sends them via LoRa to a LoRaWAN network - [Chirpstack.io](https://www.chirpstack.io/) in a set interval. 

It uses OTAA (Over-the-air activation) to connect to a LoRaWAN network which uses DevEUI for device identification and AppKey which is used to derive AppSKey (Application Session Key) and NwkSKey (Network Session Key) used for encryption of payload and calculation of MIC (Message integrity code) respectively. 

Data is encoded using [Cayenne LPP](https://github.com/myDevicesIoT/cayenne-docs/blob/master/docs/LORA.md#cayenne-low-power-payload) which encodes data in HEX format using the following schema: 

| Data1 Channel |	Data1 Type | Data1	| Data2 Channel |	Data2 Type	| Data2 | ... |
|-----------|------------|--------|-----------|-------------|-------|-----|
| 1 Byte    | 1 Byte     | N Bytes| 1 Byte    | 1 Byte      | M Bytes| ... |

There are two sketches available:
  - [Example](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/Arduino_OTAA/endNode_Arduino_OTAA_Example.ino): Reads data from [DHT11](https://learn.adafruit.com/dht) temperature and humidity sensor and sends the reading in 
  Cayenne LPP format to Chirpstack.
  - [Template](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/Arduino_OTAA/endNode_Arduino_OTAA_template.ino): Functionally same as Example, however it is missing the Cayenne LPP encoding for a specific sensors as well as the sensor's data reading function and extra libraries.

## LMiC (LoRa MAC in C)
The Arduino IBM LoRaWAN C-library is a portable implementation of the LoRaWAN™ 1.0.2 end-device specification in the C programming language (“LMiC” stands for “LoRaWAN MAC in C”). The first Arduino LMiC implementation was [LMIC-Arduino](https://github.com/matthijskooijman/arduino-lmic) which has long served as the reference implementation but it is no longer maintained. The new reference implementation is [arduino-lmic](https://github.com/mcci-catena/arduino-lmic) by MCCI Catena. We are using the former reference implementation of [LMIC-Arduino](https://github.com/matthijskooijman/arduino-lmic) because of the memory limitation of ATmega 328 (Arduino Uno) as the newest version of LMiC is memory heavy and uses almost all of the available 32K flash space.

## Prerequisites
In this section we will cover prerequisites to successfully use the sketch, such as: LMiC library modification, gateway set-up, [Chirpstack](https://www.chirpstack.io/) LoRaWAN network set-up and set-up of the Chirpstack Application server. You also need to have an IDE installed to work with Arduino, for example [Arduino IDE](https://www.arduino.cc/en/main/software). 

### LMiC library modification
Download the [LMIC-Arduino](https://github.com/matthijskooijman/arduino-lmic). Add the library in Arduino IDE: *Sketch --> Include Library --> Add .ZIP Library...*. 

You need need to change the Frequency Band to use with LG01-N. Locate a *config.h* file which path is `...\arduino\libraries\arduino-lmic \src\lmic\config.h.` and open it. Uncomment EU frequency and LG02_LG01 definition:
```C
#define CFG_eu868 1
//#define CFG_us915 1
//#define CFG_au921 1
//#define CFG_as923 1
//#define CFG_in866 1

#define LG02_LG01 1
```
 
### Gateway set-up
We are using [Dragino LG01-N Single Channel Gateway](http://www.dragino.com/products/lora/item/143-lg01n.html), though for a real world implementation, it is not recommended to use Single-channel gateways as they are not LoRaWAN-compliant.

First, set-up the LG01-N wireless connection:
  1. Connect PC to LG01-N’s LAN port via RJ45 cable. Gateway should be accesible on `10.130.1.1`
  2. Use browser to access the LG01-N via IP `10.130.1.1/cgi-bin/luci/admin`
  3. For login use *User name: root* and *Password: dragino*.
  4. In Network --> Wireless, select *radio0* interface and select Scan. 
  5. Select the desired Wireless access point, join the wifi network and enter its password.
  6. In Network --> Wireless, Disable the WiFi AP with SSID: *dragino-xxxxxx* (the one on Mode: Master)

Secondly, we need to configure the gateway so that it sends data to the ChirpStack Gateway Bridge component. In Service --> LoRaWan Gateway change the following entries:
  - IoT Service --> *LoRaWan/RAW forwarder*
  - Service Provider --> *--custom--*
  - Server Address --> YOUR SERVER'S IP ADDRESS
  - Server upstream Port --> *1700*
  - Server downstream  Port --> *1700*
  - Frequency (HZ) --> *868100000*
  - Spreading Factor --> *SF7*
  - Coding Rate --> *4/5*
  - Signal Bandwidth --> *125 kHz*

### Chirpstack servers set-up
 To be written in seperate document. For now, follow [this](https://www.chirpstack.io/guides/debian-ubuntu/) set-up.
 
### Set-up of the Chirpstack Application server
To successfully receive data by Network and Application server, we need to set-up the configuration for Network server, Service profile, Device profile, Gateway, Application and Device. To access the ChirpStack Application Server web-interface, enter the IP address or hostname of you server, followed by *port 8080* which is a default configuration, e.g. `localhost:8080`. Then for login use *Username: admin* and *Password: admin*. Guide on how to set-up the Chirpstack Application server can be also found [here](https://www.chirpstack.io/guides/first-gateway-device/).

#### Network-servers
In order to connect your ChirpStack Application Server instance with the ChirpStack Network Server instance, click Network servers and then Add. Fill a name of the Network server, so it is identifiable on the network. As the ChirpStack Network Server is installed on the same host as the ChirpStack Application Server in our case, use `127.0.0.1:8000` as *network-server server*. Then click Create Network Server.

#### Service-profiles
Click Service profiles and then Create. Fill a name for a service profile for easier identification. Pick a Network server you would like to associate with this service profile. Maximum allowed data-rate should be set to 5. Then click Create Service profile.

#### Device-profiles
Click Device profiles and then Create. In *General* tab, fill a name for a device profile and pick a network server to associate it with. Then set:
  - LoRaWAN MAC version --> *1.0.3*
  - LoRaWAN Regional parameters version --> *B*

In *Join (OTAA/ABP)* tab *Check* Device supports OTAA box. In *Codec* tab pick *Cayenne LPP*. Then click Create Device profile.

#### Gateways
Click Gateways and then Create. Fill a Gateway name and description. For the Gateway ID you can choose your own (but do not forget to update it on a Gateway itself as well) or access the Gateway through browser on `10.130.1.1/cgi-bin/luci/admin` (if connected via RJ45 cable). Go to Services --> LoRaWan Gateway and copy the *Gateway ID* from there. Then choose a Network server you would like the gateway to connect to. Then click Create Gateway.

#### Applications
Click Applications and then Create. Fill Application name and description. Pick a Service profile to associate the application with. Then click Create Application.
 
#### Devices
Click Applications, pick one of applications from the list and then Create. Fill Device name and description. Generate new Device EUI (DEVEUI) or fill your own. Pick a Device profile to associate with a device. Then click Create Device.

Open a device you just created and go to *Keys (OTAA)* tab. Generate new Application key (APPKEY) or fill your own. Then click Set Device-keys.

## Installation
For the Template sketch:
  - Download the sketch and modify it according to your needs.

For the Example sketch:
  - Download the sketch.
  - Connect the sensor's pins to Arduino as follows: 

| DHT 11 Sensor | Arduino Uno |
|---------------|-------------|
| GND           | GND         |
| DAT           | A0          |
| VCC           | 5V          |

  - Download DHT 11 library:
    - Tools --> Manage Libraries --> Search for DHT 11 and install, or
    - Download the [DHT 11 library](https://github.com/goodcheney/Lora/blob/patch-1/Lora%20Shield/Examples/DHTlib.zip). Add the library in Arduino IDE: *Sketch --> Include Library --> Add .ZIP Library...*.
  - Open your Chirpstack application server in a browser (default `localhost:8080`), navigate to *Applications* and pick a desired application from the list.
  - Open the sketch and update:
    - *DEVEUI*: In the application server, from the list of applications open a desired application. In the list of devices, find a desired device and copy its Device EUI. The value of DEVEUI must be in the HEX format (`0x12, 0x34, 0x56, ...`). Once in HEX format, we need to supply the DEVEUI with LSB (least significant bit) first. Therefore we need to rewrite it to `..., 0x56, 0x34, 0x12`. In the sketch, paste its value to `PROGMEM DEVEUI`.
    - *APPKEY*: From the list of devices, pick a desired device. Open *Keys (OTAA)* tab, uncover the Application key and copy it in HEX Array format. In the sketch, paste its value to `PROGMEM APPKEY`.
  - Save the sketch.
  - Load the sketch to Arduino.
  - Arduino joins the network and data can be seen on Chirpstack Application server under Applications/APPLICATION_NAME/Devices/DEVICE_NAME.
