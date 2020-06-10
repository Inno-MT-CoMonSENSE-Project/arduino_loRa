# MQTT-to-MongoDB
Node.js application that listens to MQTT topic and insert data to MongoDB.

## Prerequisites
You need MongoDB and Node.js installed and running MQTT broker.

MongoDB can be installed by following the guidline on how to install 
[MongoDB](https://www.digitalocean.com/community/tutorials/how-to-install-mongodb-on-ubuntu-18-04)
on Ubuntu or via terminal using:
```shell
$ sudo apt install -y mongodb
```

Node.js can be installed by following the guidline on how to install 
[node.js](https://www.digitalocean.com/community/tutorials/how-to-install-node-js-on-ubuntu-18-04)
on Ubuntu or via terminal using:
```shell
$ sudo apt-get install nodejs
```

MQTT broker is installed together with a Chirpstack.io, the protocol is [Mosquitto](https://mosquitto.org/).

You may also want to install **npm**, the Node.js package manager.
```shell
$ sudo apt-get install npm
```

## Installation
Clone the repository and modify following files:

  - Update dependencies in **package.json** to versions of MQTT and MongoDB you are using.
  - Update the configuration for MQTT and MongoDB in **config.js** based on your setup

Run **server.js**
```shell
$ cd MQTT-to-MongoDB
$ node server.js
```
