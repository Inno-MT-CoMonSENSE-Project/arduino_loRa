// Import libraries. Need to add libraries for sensor if needed.
#include <lmic.h>
#include <hal/hal.h>

// APPEUI - 64-bit application identifier. Must be in little-endian format, so least-significant-byte first. 
// LoRa App Server acts as a default join-server currently, (regardless of the AppEUI value), 
// the AppEUI / JoinEUI (two names, same field) does not matter.
static const u1_t PROGMEM APPEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
void os_getArtEui (u1_t* buf) { memcpy_P(buf, APPEUI, 8);} 

// DEVEUI - unique 64-bit end-device identifier. Must be in little-endian format. 
// Can be genereted by Chirsptack's Application server or use own.
static const u1_t PROGMEM DEVEUI[8]={ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // --> Unique for each end-node
void os_getDevEui (u1_t* buf) { memcpy_P(buf, DEVEUI, 8);} 

// APPKEY - AES-128 root key unique for the end-device. Must be in big endian format.
// Used to derive the session keys NwkSKey and AppSKey specific for that end-device to encrypt 
// and verify network communication and application data. Can be genereted by Chirsptack's Application server or use own.
static const u1_t PROGMEM APPKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 }; // --> Unique for each end-node
void os_getDevKey (u1_t* buf) { memcpy_P(buf, APPKEY, 16);}

// Setting Cayenne LPP format. Need to choose a channel (optional) and data type for each of the sensors' data. 
// More info about a format here: https://developers.mydevices.com/cayenne/docs/lora/
static uint8_t LPP_data[ ] = { 0x00,0x00,0x00,0x00 };

// Per job control struct, which identifies the job and stores context information.
static osjob_t sendjob;

// Schedule TX every this many seconds (might become longer due to duty cycle limitations).
const unsigned TX_INTERVAL = 30;

// Pin mapping
const lmic_pinmap lmic_pins = {
    .nss = 10,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 9,
    .dio = {2, 6, 7},
};

// Application’s callback function which prints Events via Serial.
void onEvent (ev_t ev) {
    Serial.print(os_getTime());
    Serial.print(": ");
    switch(ev) {
        case EV_SCAN_TIMEOUT:
            Serial.println(F("EV_SCAN_TIMEOUT"));
            break;
        case EV_BEACON_FOUND:
            Serial.println(F("EV_BEACON_FOUND"));
            break;
        case EV_BEACON_MISSED:
            Serial.println(F("EV_BEACON_MISSED"));
            break;
        case EV_BEACON_TRACKED:
            Serial.println(F("EV_BEACON_TRACKED"));
            break;
        case EV_JOINING:
            Serial.println(F("EV_JOINING"));
            break;
        case EV_JOINED:
            Serial.println(F("EV_JOINED"));
            break;
        case EV_RFU1:
            Serial.println(F("EV_RFU1"));
            break;
        case EV_JOIN_FAILED:
            Serial.println(F("EV_JOIN_FAILED"));
            break;
        case EV_REJOIN_FAILED:
            Serial.println(F("EV_REJOIN_FAILED"));
            break;
        case EV_TXCOMPLETE:
            Serial.println(F("EV_TXCOMPLETE (includes waiting for RX windows)"));
            if (LMIC.txrxFlags & TXRX_ACK)
              Serial.println(F("Received ack"));
            if (LMIC.dataLen) {
              Serial.println(F("Received "));
              Serial.println(LMIC.dataLen);
              Serial.println(F(" bytes of payload"));
            }
            // Schedule next transmission in TX_INTERVAL time.
            os_setTimedCallback(&sendjob, os_getTime()+sec2osticks(TX_INTERVAL), do_send);
            break;
        case EV_LOST_TSYNC:
            Serial.println(F("EV_LOST_TSYNC"));
            break;
        case EV_RESET:
            Serial.println(F("EV_RESET"));
            break;
        case EV_RXCOMPLETE:
            // data received in ping slot
            Serial.println(F("EV_RXCOMPLETE"));
            break;
        case EV_LINK_DEAD:
            Serial.println(F("EV_LINK_DEAD"));
            break;
        case EV_LINK_ALIVE:
            Serial.println(F("EV_LINK_ALIVE"));
            break;
         default:
            Serial.println(F("Unknown event"));
            break;
    }
}

// Read sensor value and make data ready for transmission.
void do_send(osjob_t* j){
    // Check if there is not a current TX/RX job running
    if (LMIC.opmode & OP_TXRXPEND) {
        Serial.println(F("OP_TXRXPEND, not sending"));
    } else {
        // Read sensor value. Code to be written dependig on a sensor. Missing here.
        // --- Write code here!
        // Prepare upstream data transmission at the next possible time.
        LMIC_setTxData2(1, LPP_data, sizeof(LPP_data), 0);
        Serial.println(F("Packet queued"));
    }
    // Next TX is scheduled after TX_COMPLETE event.
}

// Called when a sketch starts to initialize variables, pin modes, start using libraries, etc. 
// Only run once, after each powerup or reset of the Arduino board.
void setup() {
    Serial.begin(9600);
    Serial.println(F("Starting"));

    // Initialize the run-time environment 
    os_init();
    // Reset the MAC state. Session and pending data transfers will be discarded.
    LMIC_reset();
    // Start job
    do_send(&sendjob);
}

// Loops consecutively, allowing the program to change and respond.
void loop() {
    // Execute run-time jobs from the timer and from the run queues. Returns after dispatching the first available job.
    os_runloop_once();
}
