# CoMonSENSE-Project
BEING UPDATED (27/07/2020)

MISSING:
 - Intro
## General set-up
The system can be broken into the following components:
  - End node
  - Gateway
  - Server
  - Database

These components are depicted in the diagram below:
![System_architecture](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/commenSense_ach.jpg)

### End node
The end node is the LoRaWAN device which sends data to the ChirpStack Network Server through a Gateway. We use [Arduino UNO](https://store.arduino.cc/arduino-uno-rev3) with [Lora Shield](https://www.dragino.com/products/lora/item/102-lora-shield.html) for an end node. The shield allows us to send data via LoRa to a gateway. It makes use of the LMIC library (portable implementation of the LoRa MAC specification) to provide protocol compliance. Sensors are connected to the Arduino (measuring temperature, humidity, etc.) and readings are sent over LoRa in [Cayenne LPP](https://github.com/myDevicesIoT/cayenne-docs/blob/master/docs/LORA.md#cayenne-low-power-payload) format.

### Gateway
A LoRaWAN compliant Gateway listens to 8 or more channels simultaneously and forwards received data (from end nodes) to the network server. There is a software running on the gateway - Packet forwarder, responsible for receiving and sending. We use single channel gateway - [Dragino LG01-N](http://www.dragino.com/products/lora/item/143-lg01n.html) which can only receive on one channel and one spreading factor at the same time which makes it not LoRaWAN compliant and only suitable for prototyping.

### Server
Four components are residing on the server:
 - Chirpstack Gateway bridge
 - MQTT broker
 - Network server
 - Application server
 
#### Chirpstack Gateway bridge
It is a service that sits between the Packet forwarder and MQTT broker and handles the communication with gateways. Its main responsibility is to convert the Packet Forwarder format into a ChirpStack Network Server common data-format (JSON and Protobuf). It can reside directly on the gateway itself or the server, as in our setup.

#### MQTT broker
MQTT is a bi-directional communication protocol. The broker acts as a post office. The recipient subscribes to a given Topic and will receive all messages published under this topic. It offers one to many capability - multiple clients can receive the message from a single broker, as well as many to one - multiple publishers can publish topics to a single subscriber. Each client can therefore both produce and receive data by both publishing and subscribing.

#### Network server
The ChirpStack Network Server is a LoRaWAN Network Server, responsible for managing the state of the network. It has knowledge of device activations on the network and is able to handle join-requests when devices want to join the network. The main responsibility of the network server is de-duplication of received LoRaWAN frames and handling of authentication, LoRaWAN mac-layer (and mac-commands), communication with the ChirpStack Application Server, or scheduling of downlink frames.

#### Application server
The ChirpStack Application Server is a LoRaWAN Application Server providing a web-interface and APIs for the management of users, organizations, applications, gateways, and devices. The data it receives is forwarded to configured integrations. It is responsible for the device "inventory" part of a LoRaWAN infrastructure, handling of join-request, and the handling and encryption of application payloads.

### Database
The application server does not provide persistent data storage, it only caches the received data. [MongoDB](https://www.mongodb.com/) is a database of choice in this project.


DON'T FORGET THE BASIC FLOW AMONG ENTITIES



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
