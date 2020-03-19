# CoMonSENSE-Project

## General set-up (Chirpstack)
As it is described here https://www.chirpstack.io/overview/architecture/, there are two option on where the Chirpstack Gateway Bridge can reside - either on the gateway itself or the server. In our setup, the GW Bridge is on the server (on the local machine). It is between the Packet forwarder and MQTT broker. 

All installation steps were done according to this tutorial: https://www.chirpstack.io/guides/debian-ubuntu/.

The configuration of the GW Bridge was not changed.

In the PostgreSQL two roles, one for network and one for application server, were created along with associated passwords. Extensions created accordingly as well. 

The network server’s configuration was changed according to the tutorial using the role and password created in the PostgreSQL and frequencies were setup based on *EU868*.

Similarly, the application server’s configuration was modified according to the tutorial using the role and password created in the PostgreSQL. JWT secret was generated and provided also.

Configuration files for every entity respectively, can be found in *Chirpstack_config_files* folder.

Then we have configured the LoRaWAN server settings and chose LoRaWAN forwarder as an IoT service and configured its server address (corresponding IP) and ports (port: *1700*). Also, the frequency of the radio is set to *868100000*.

### Application server configuration
Once all entities are configured, there is a need to properly set-up the application server. The application server on http://localhost:8080/ with the following credentials:

  - Username: *admin*
  - Password: *admin*

The following needs to be created:
  - Network server at: *localhost:8080*
  - Service profile with previously created network server attached to it.
  - Device profile:
    - LoRaWAN MAC version is set to *1.0.2*
    - LoRaWAN Regional Parameters version is set to *B*
    - In CLASS-B tab, the fields are: *checked*, *30*, *every 8 seconds*, *5*, *1*
    - In CLASS-C tab, the fields are: *checked*, *60*
    - In CODEC tab, the Payload Codec is set to *Cayenne LPP*
  - Gateway, where as a Gateway ID we use the ID of given Gateway
  - Application with a Service profile associated.
    - Device within an application needs to be created with Frame-counter validation *disabled*, Device profile associated and either given or randomly generated Device EUI.
    - The newly created device need to have an Activation configured, where both Network session key and Application session key are generated randomly. ??? Uplink and Downlink frame-counter were configured with random values ??? 

Once the Application server is configured, we can upload a sketch to Arduino.

The following were used as inspiration and tutorials:
  - https://www.chirpstack.io/guides/first-gateway-device/
  - https://www.youtube.com/watch?v=mkuS5QUj5Js

## Measure temperature and humidity and send in Cayenne LPP form
The *temp_humidity_LPP.ino* is a sketch which measures temperature and humidity of surroundings, encode data to Cayenne LPP format and send them to the gateway and up to application server.

There is a need to update the NwkSKey - network session key, AppSKey - application session key and DevAddr - end-device address with values chosen/generated during device Activation configuration in the previous step.
