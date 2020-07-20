# CoMonSENSE-Project
THIS README NEEDS UPDATE (18.6.)

MISSING:
 - Intro
## General set-up (Chirpstack)
The system can be broken into the following components:
  - end node
  - gateway
  - server
  - database

These components are depicted in the diagram below:
![System_architecture](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/commenSense_ach.jpg)


As it is described here https://www.chirpstack.io/overview/architecture/, there are two option on where the Chirpstack Gateway Bridge can reside - either on the gateway itself or the server. In our setup, the GW Bridge is on the server (on the local machine). It is between the Packet forwarder and MQTT broker. 

System's architecture along with IPs and ports entities, can be found in *cms_architecture.png*.

All installation steps were done according to this tutorial: https://www.chirpstack.io/guides/debian-ubuntu/.

The configuration of the GW Bridge was not changed.

In the PostgreSQL two roles, one for network and one for application server, were created along with associated passwords. Extensions created accordingly as well. 

The network server’s configuration was changed according to the tutorial using the role and password created in the PostgreSQL and frequencies were setup based on *EU868*.

Similarly, the application server’s configuration was modified according to the tutorial using the role and password created in the PostgreSQL. JWT secret was generated and provided also.

Configuration files for every entity respectively, can be found in *Chirpstack_config_files* folder.

Then we have configured the LoRaWAN server settings and chose LoRaWAN forwarder as an IoT service and configured its server address (corresponding IP) and ports (port: *1700*). Also, the frequency of the radio is set to *868100000*.


    -  When registering a new device, Uplink and Downlink frame-counter are set to 0. More on the matter of frame counters here: https://www.thethingsnetwork.org/docs/lorawan/security.html#frame-counters

Once the Application server is configured, we can upload a sketch to Arduino.

The following were used as inspiration and tutorials:
  - https://www.chirpstack.io/guides/first-gateway-device/
  - https://www.youtube.com/watch?v=mkuS5QUj5Js

## Measure temperature and humidity and send in Cayenne LPP form
The *temp_humidity_LPP.ino* is a sketch which measures temperature and humidity of surroundings, encode data to Cayenne LPP format and send them to the gateway and up to application server.

There is a need to update the NwkSKey - network session key, AppSKey - application session key and DevAddr - end-device address with values chosen/generated during device Activation configuration in the previous step.

# Arduino-LMIC library
According to this forum (https://www.thethingsnetwork.org/forum/t/overview-of-lorawan-libraries-howto/24692), the first Arduino LMIC implementation was LMIC-Arduino (https://github.com/matthijskooijman/arduino-lmic) which has long served as the reference implementation but it is no longer maintained. The new reference implementation is https://github.com/mcci-catena/arduino-lmic. 

As explained in readme of the new reference implemenation, we might have a problems with memory available as:
  - This library can be quite heavy on small systems, especially if the fairly small ATmega 328p (such as in the Arduino Uno) is used. In the default configuration, the available 32K flash space is nearly filled up (this includes some debug output overhead, though). By disabling some features in project_config/lmic_project_config.h (like beacon tracking and ping slots, which are not needed for Class A devices), some space can be freed up.
