# Arduino based end-node sending data over LoRa to LoRaWAN network
This sketch reads sensor's data, encode them to Cayanne LPP format and sends them via LoRa to a LoRaWAN network - [Chirpstack.io](https://www.chirpstack.io/) in a set interval.  It uses OTAA (Over-the-air activation) to join a LoRaWAN network and a set of keys to encrypt the communication between the end-node and Application server.

There are two sketches available:
  - Example: Reads data from [DHT11](https://learn.adafruit.com/dht) temperature and humidity sensor and sends the reading in 
  Cayanne LPP format to Chirpstack.
  - Template: Functionally same as Example, however it is missing the Cayanne LPP encoding for a specific sensors as well as the sensor's data reading function and extra libraries.

## Over-the-Air Activation (OTAA)

This uses OTAA (Over-the-air activation), where where a DevEUI and
application key is configured, which are used in an over-the-air
activation procedure where a DevAddr and session keys are
assigned/generated for use with all further communication.

## LMiC (LoRa MAC in C)

## Cayanne LPP

## Prerequisites
You need to have [Arduino IDE](https://www.arduino.cc/en/main/software) installed, gateway running, [Chirpstack](https://www.chirpstack.io/) LoRaWAN network set-up and have a device registered on Chirpstack Application Server.
 
 ### Gateway set-up
 We are using [Dragino LG01-N Single Channel Gateway](http://www.dragino.com/products/lora/item/143-lg01n.html), though for a real world implementation, it is not recommended to use Single-channel gateways as they are not LoRaWAN-compliant.

  1. Connect PC to LG01-N’s LAN port via RJ45 cable. Gateway should be accesible on `10.130.1.1`
  2. Use browser to access the LG01-N via IP `10.130.1.1/cgi-bin/luci/admin`
  
`s`
 
 ### Chirpstack servers set-up
 To be written in seperate document. For now, follow [this](https://www.chirpstack.io/guides/debian-ubuntu/) set-up.
 
 ### Device registration
To access the ChirpStack Application Server web-interface, enter the IP address or hostname of you server, followed by *port 8080* which is a default configuration.


 - setup on Chirpstack
 
register your application and device with
the things network, to set or generate an AppEUI, DevEUI and AppKey.
Multiple devices can use the same AppEUI, but each device has its own
DevEUI and AppKey.

## Installation
Clone the repository and modify the Template sketch accoring to your needs. 

For the Example sketch:
  - Connect the sensor's pins to Arduino as follows: 

| DHT 11 Sensor | Arduino Uno |
|---------------|-------------|
| GND           | GND         |
| DAT           | A0          |
| VCC           | 5V          |

  - Download DHT 11 library:
    - Tools --> Manage Libraries --> Search for DHT 11 and install, or
    - Download the [DHT 11 library](https://github.com/goodcheney/Lora/blob/patch-1/Lora%20Shield/Examples/DHTlib.zip) manually, unzip it and put it to Arduino installation location to \Arduino\libraries directory.
  - Load the Example sketch to Arduino.
  - Arduino joins the network and data can be seen on Chirpstack Application server under Applications/APPLILCATION_NAME/Devices/DEVICE_NAME
