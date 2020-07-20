# MQTT-to-MongoDB
Node.js application that listens to MQTT topic and insert data to MongoDB.

## Prerequisites
You need MongoDB and Node.js installed and running MQTT broker.

MongoDB can be installed by following the guidline on how to install 
[MongoDB](https://docs.mongodb.com/manual/tutorial/install-mongodb-on-ubuntu/)
on Ubuntu.

Node.js can be installed by following the guidline on how to install 
[node.js](https://github.com/nodesource/distributions/blob/master/README.md)
on Ubuntu or via terminal using (for the LTS version):
```shell
curl -sL https://deb.nodesource.com/setup_lts.x | sudo -E bash -
sudo apt-get install -y nodejs
```

MQTT broker is installed together with a Chirpstack.io, the protocol is [Mosquitto](https://mosquitto.org/).

You may also want to install ```npm```, the Node.js package manager.
```shell
sudo apt-get install npm
```

## Installation
Clone the repository and modify the following:
  
  - Update the configuration for MQTT and MongoDB in **config.js** based on your setup.

Change your current directory to *MQTT-to-MongoDB* and install dependencies:

```shell
cd MQTT-to-MongoDB
sudo npm install mongodb
sudo npm install mqtt
``` 

Run **server.js**
```shell
node server.js
```
