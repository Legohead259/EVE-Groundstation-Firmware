#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>

// LoRa Instantiation
#define RFM_CS_PIN 38
#define RFM_RST_PIN 0
#define RFM_IRQ_PIN 33

SPIClass LoRaSPI(FSPI);

void setup() {
	Serial.begin(115200);
	while(!Serial); // Wait for serial connection to open

	// Initialize LoRa Radio
	Serial.print("Initializing radio...");
    LoRa.setPins(RFM_CS_PIN, RFM_RST_PIN, RFM_IRQ_PIN);
    LoRaSPI.begin(37, 36, 35, 38);
    LoRa.setSPI(LoRaSPI);
    
    if (!LoRa.begin(915E6)) {
        Serial.println("Starting LoRa failed!");
        // while(true) blinkCode((byte) RADIO_ERROR_CODE, RED); // Block further code execution
    }
    LoRa.setSyncWord(0xF3);
    Serial.println("done!");

	// Initialize filesystem
	// #define SDCARD_DEBUG
	// if (!initSDCard()) { // Initialize filesystem and check if good
    //     // while(true) blinkCode(CARD_MOUNT_ERROR_CODE); // Block further code execution
    // }
}

void loop() {
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        Serial.print("Received packet '");

        // read packet
        while (LoRa.available()) {
        Serial.print((char)LoRa.read(), HEX);
        }

        // print RSSI of packet
        Serial.print("' with RSSI ");
        Serial.println(LoRa.packetRssi());
    }
}