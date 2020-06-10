// Define dependencies and modules
var mongodb  = require('mongodb');
var mqtt     = require('mqtt');
var config   = require('./config');

// Define MQTT URL and client
var mqttUrl  = 'mqtt://' + config.mqtt.hostname + ':' + config.mqtt.port;
var client   = mqtt.connect(mqttUrl);

// Subscribe to the MQTT topic on connection
client.on('connect', function () {
    client.subscribe(config.mqtt.topic);
});

// Define MongoDB URL
var mongoUrl = 'mongodb://' + config.mongodb.hostname + ':' + config.mongodb.port + '/' + config.mongodb.database;

// Connect to MongoDB on given URL
mongodb.MongoClient.connect(mongoUrl, function(error, database) {
    if(error != null) {
        throw error;
	}

// Specify collection
    var collection = database.collection(config.mongodb.collection);

// When new message arrives to MQTT broker, parse its content to JSON
    client.on('message', function (topic, message) {
        var dataFromNode = JSON.parse(message);
// Date and ID formatting
        dataFromNode.rxInfo[0].time = new Date(dataFromNode.rxInfo[0].time);
        dataFromNode.applicationID = parseInt(dataFromNode.applicationID);
// Data to be inserted into the DB put into objects
        var messageObject = {
            topic: topic,
            deviceData: dataFromNode
        };

// Insert data from node to a specified collection
        collection.insert(messageObject, function(error, result) {
            if(error != null) {
                console.log("ERROR: " + error);
            }
	    });
    });
});
