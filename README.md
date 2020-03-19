# arduino_loRa
LoRa sheild_sketch
Chirpstack.io – CoMonSENSE

As it is described here https://www.chirpstack.io/overview/architecture/, there are two option on where the Chirpstack Gateway Bridge can be residing either on the gateway itself or the server. In our setup, the GW Bridge is on the server (on the local machine). It is between the Packet forwarder and MQTT broker. 

All installation steps were done according to this tutorial: https://www.chirpstack.io/guides/debian-ubuntu/.

The configuration of the GW Bridge was not changed.

In the PostgreSQL two roles, one for network -server and one for application-server, were created along with associated passwords. Extensions created accordingly as well. 

The network server’s configuration was changed according to the tutorial using the role and password created in the PostgreSQL and frequencies were setup based on EU868.

Similarly, the application server’s configuration was modified according to the tutorial using the role and password created in the PostgreSQL. JWT secret was generated and provided also.

Now all servers are setup and we can access the application server on http://localhost:8080/ with username: admin and password: admin.

Then we have configured the LoRaWAN server settings and chose LoRaWAN forwarder as an IoT service and configured its server address (corresponding IP) and ports (value: 1700). Also, the frequency of the radio is set to 868100000.

More details check the sketch.

