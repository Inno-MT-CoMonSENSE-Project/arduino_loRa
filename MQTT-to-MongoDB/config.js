var config = {};

config.debug = process.env.DEBUG || false;

// MQTT configuration - topic for subscription, address, port 
config.mqtt  = {};
config.mqtt.topic = process.env.mqtt_topic 	   || 'application/2/#';
config.mqtt.hostname  = process.env.mqtt_hostname  || 'localhost';
config.mqtt.port      = process.env.mqtt_port      || 1883;

// MongoDB configuration - address, port, database name, collection name
config.mongodb = {};
config.mongodb.hostname   = process.env.mongodb_hostname   || 'localhost';
config.mongodb.port       = process.env.mongodb_port       || 27017;
config.mongodb.database   = process.env.mongodb_database   || 'CommonSense';
config.mongodb.collection = process.env.mongodb_collection || 'data';

module.exports = config;
