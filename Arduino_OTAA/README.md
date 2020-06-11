# Arduino based end-node sending data over LoRa to LoRaWAN network
This sketch reads sensor's data, encode them to Cayanne LPP format and sends them via LoRa to a LoRaWAN network - 
[Chirpstack.io](https://www.chirpstack.io/) in a set interval.  It uses OTAA (Over-the-air activation) to join a LoRaWAN network and a set of keys 
to encrypt the communication between the end-node and Application server.

There are two sketches available:
  - Template:
  - Example: Reads data from [DHT11](https://learn.adafruit.com/dht) temperature and humidity sensor and sends the readings in 
  Cayanne LPP format to Chirpstack.
## Over-the-Air Activation (OTAA)

This uses OTAA (Over-the-air activation), where where a DevEUI and
application key is configured, which are used in an over-the-air
activation procedure where a DevAddr and session keys are
assigned/generated for use with all further communication.

## LMiC (LoRa MAC in C)

## Cayanne LPP

## Prerequisites
  - setup on Chirpstack
 register your application and device with
the things network, to set or generate an AppEUI, DevEUI and AppKey.
Multiple devices can use the same AppEUI, but each device has its own
DevEUI and AppKey.

## Installation
  - Explain template and example

frequency and encryption settings matching those of
the The Things Network.


two files
  - template
  - example
