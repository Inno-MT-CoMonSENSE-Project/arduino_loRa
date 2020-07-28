# CoMonSENSE-Project
BEING UPDATED (28/07/2020)

MISSING:
 - Intro about the project
# General set-up
The system can be broken into the following components:
  - End node
  - Gateway
  - Server
  - Database

These components are depicted in the diagram below:
![System_architecture](arduino_loRa/commenSense_ach.jpg)

## End node
The end node is the LoRaWAN device which sends data to the ChirpStack Network Server through a Gateway. We use [Arduino UNO](https://store.arduino.cc/arduino-uno-rev3) with [Lora Shield](https://www.dragino.com/products/lora/item/102-lora-shield.html) for an end node. The shield allows us to send data via LoRa to a gateway. It makes use of the LMIC library (portable implementation of the LoRa MAC specification) to provide protocol compliance. Sensors are connected to the Arduino (measuring temperature, humidity, etc.) and readings are sent over LoRa in [Cayenne LPP](https://github.com/myDevicesIoT/cayenne-docs/blob/master/docs/LORA.md#cayenne-low-power-payload) format.

## Gateway
A LoRaWAN compliant Gateway listens to 8 or more channels simultaneously and forwards received data (from end nodes) to the network server. There is a software running on the gateway - Packet forwarder, responsible for receiving and sending. We use single channel gateway - [Dragino LG01-N](http://www.dragino.com/products/lora/item/143-lg01n.html) which can only receive on one channel and one spreading factor at the same time which makes it not LoRaWAN compliant and only suitable for prototyping.

## Server
Four components are residing on the server:
 - Chirpstack Gateway bridge
 - MQTT broker
 - Network server
 - Application server
 
### Chirpstack Gateway bridge
It is a service that sits between the Packet forwarder and MQTT broker and handles the communication with gateways. Its main responsibility is to convert the Packet Forwarder format into a ChirpStack Network Server common data-format (JSON and Protobuf). It can reside directly on the gateway itself or the server, as in our setup.

### MQTT broker
MQTT is a bi-directional communication protocol (publish/subscribe). The broker acts as a post office. The recipient subscribes to a given Topic and will receive all messages published under this topic. It offers one to many capability - multiple clients can receive the message from a single broker, as well as many to one - multiple publishers can publish topics to a single subscriber. Each client can therefore both produce and receive data by both publishing and subscribing.

### Network server
The ChirpStack Network Server is a LoRaWAN Network Server, responsible for managing the state of the network. It has knowledge of device activations on the network and is able to handle join-requests when devices want to join the network. The main responsibility of the network server is de-duplication of received LoRaWAN frames and handling of authentication, LoRaWAN mac-layer (and mac-commands), communication with the ChirpStack Application Server, or scheduling of downlink frames.

### Application server
The ChirpStack Application Server is a LoRaWAN Application Server providing a web-interface and APIs for the management of users, organizations, applications, gateways, and devices. The data it receives is forwarded to configured integrations. It is responsible for the device "inventory" part of a LoRaWAN infrastructure, handling of join-request, and the handling and encryption of application payloads.

### Database
The application server does not provide persistent data storage, it only caches the received data. [MongoDB](https://www.mongodb.com/) is a database of choice in this project.

# Installation
The installation is performed on a single machine running an Ubuntu distribution of Linux. Chirpstack also provides a quickstart [tutorial](https://www.chirpstack.io/project/guides/debian-ubuntu/) for setting up a Chirpstack stack.

The installation can be broken into the installation of:
 - Dependencies
 - ChirpStack repository
 - Gateway bridge
 - Network server
 - Application server

## Dependencies
The following dependencies need to be installed: 
 - MQTT broker - a bi-directional communication protocol (publish/subscribe).
 - Redis - An in-memory database used to store relatively transient data.
 - PostgreSQL - The long-term storage database used by the open-source packages.

```shell
sudo apt install mosquitto mosquitto-clients redis-server redis-tools postgresql
```
To setup PostgreSQL databases and users enter the command-line utility for PostgreSQL:

```shell
sudo -u postgres psql
```
There, execute the following queries to set up the databases that are used by the ChirpStack stack components.

```sql
-- set up the users and the passwords
-- (note that it is important to use single quotes and a semicolon at the end!)
create role chirpstack_as with login password 'dbpassword';
create role chirpstack_ns with login password 'dbpassword';

-- create the database for the servers
create database chirpstack_as with owner chirpstack_as;
create database chirpstack_ns with owner chirpstack_ns;

-- change to the ChirpStack Application Server database
\c chirpstack_as

-- enable the pq_trgm and hstore extensions
-- (this is needed to facilitate the search feature)
create extension pg_trgm;
-- (this is needed to store additional k/v meta-data)
create extension hstore;

-- exit psql
\q
```
Do not forget to change *usernames* and *passwords*. Also, the entered *usernames* and *passwords* will be later used in ```chirpstack-network-server.toml``` and ```chirpstack-application-server.toml``` when updating the configuration files. 

## Setup Chirpstack software repository
ChirpStack provides a repository that is compatible with the Ubuntu ```apt``` package system. First, make sure that both ```dirmngr``` and ```apt-transport-https``` are installed and then set-up a key for the new repository:

```shell
sudo apt install apt-transport-https dirmngr
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 1CE2AFD36DBCCA00
```
Add the repository to the repository list and update apt package cache:
```shell
sudo echo "deb https://artifacts.chirpstack.io/packages/3.x/deb stable main" | sudo tee /etc/apt/sources.list.d/chirpstack.list
sudo apt update
```

## Gateway bridge
The gateway bridge is run on the server, not on a gateway itself. Install it using:

```shell
sudo apt install chirpstack-gateway-bridge
```
Then start the service:
```shell
# start chirpstack-gateway-bridge
sudo systemctl start chirpstack-gateway-bridge

# start chirpstack-gateway-bridge on boot
sudo systemctl enable chirpstack-gateway-bridge
```
The configuration file is located at ```/etc/chirpstack-gateway-bridge/chirpstack-gateway-bridge.tom```, but there is no need to change the configuration.
## Network server
Install the package:
```shell
sudo apt install chirpstack-network-server
```
The configuration file is located at ```/etc/chirpstack-network-server/chirpstack-network-server.toml``` and must be updated according to the band used (EU868) and database configuration ([this step](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/README.md#dependencies), need to update  the *chirpstack_ns:dbpassword*.
```toml
[general]
log_level=4

[postgresql]
dsn="postgres://chirpstack_ns:dbpassword@localhost/chirpstack_ns?sslmode=disable"

[network_server]
net_id="000000"

  [network_server.band]
  name="EU_863_870"

  [[network_server.network_settings.extra_channels]]
  frequency=867100000
  min_dr=0
  max_dr=5

  [[network_server.network_settings.extra_channels]]
  frequency=867300000
  min_dr=0
  max_dr=5

  [[network_server.network_settings.extra_channels]]
  frequency=867500000
  min_dr=0
  max_dr=5

  [[network_server.network_settings.extra_channels]]
  frequency=867700000
  min_dr=0
  max_dr=5

  [[network_server.network_settings.extra_channels]]
  frequency=867900000
  min_dr=0
  max_dr=5
  ```
Then start the service:
```shell
# start chirpstack-network-server
sudo systemctl start chirpstack-network-server

# start chirpstack-network-server on boot
sudo systemctl enable chirpstack-network-server
```
To see if everything is running correctly, you can print the ChirpStack Network server log-output:
```shell
sudo journalctl -f -n 100 -u chirpstack-network-server
```

## Application server
Install the package:
```shell
sudo apt install chirpstack-application-server
```

The configuration file is located at ```/etc/chirpstack-application-server/chirpstack-application-server.toml``` and must be updated according to the database configuration ([this step](https://github.com/Inno-MT-CoMonSENSE-Project/arduino_loRa/blob/master/README.md#dependencies), need to update  the *chirpstack_as:dbpassword*. Also, the ```jwt_secret``` must be replaced with a random secret. ```openssl rand -base64 32``` can be used to generate one.

```toml
[general]
log_level=4

[postgresql]
dsn="postgres://chirpstack_as:dbpassword@localhost/chirpstack_as?sslmode=disable"

  [application_server.external_api]
  jwt_secret=" "
```
Then start the service:
```shell
# start chirpstack-application-server
sudo systemctl start chirpstack-application-server

# start chirpstack-application-server on boot
sudo systemctl enable chirpstack-application-server
```
To see if everything is running correctly, you can print the ChirpStack Application Server log-output:
```shell
sudo journalctl -f -n 100 -u chirpstack-application-server
```
After installing the full ChirpStack stack, you should be able to navigate to the ChirpStack Application server web-interface, which can be accessed on ```http://localhost:8080``` (or IP address of the server followed by port ```8080```). For setting up a device and gateway [continue here](Arduino_OTAA/README.md).
