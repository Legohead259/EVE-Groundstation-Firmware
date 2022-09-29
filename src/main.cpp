#include "helpers/helper.h"
#include <LoRa.h>

void setup() {
	Serial.begin(115200);
	while(!Serial); // Wait for serial connection to open

	// Initialize LoRa Radio
	Serial.print("Initializing radio...");
    LoRa.setPins(RFM_CS_PIN, RFM_RST_PIN, RFM_IRQ_PIN);
    LoRaSPI.begin(FSPI_SCLK_PIN, FSPI_MISO_PIN, FSPI_MOSI_PIN, FSPI_CS_PIN);
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
    Serial.println("Timestamp (ISO8601),voltage,GPSFix,numSats,HDOP,latitude (°),longitude (­°),speed (kts),course (°),barometer temp (°C),pressure (Pa),altitude AGL (m),sysCal,gyroCal,accelCal,magCal,accelX (m/s),accelY (m/s),accelZ (m/s),gyroX (rad/s),gyroY (rad/s),gyroZ (rad/s), roll (°), pitch (°), yaw (°),linAccelX (m/s),linAccelY (m/s),linAccelZ (m/s),imu temp (°C),sht temp (°C),humidity (%),state,packet size (b),period (s),frequency (Hz),RSSI (dB),dX (m),dY (m),displacement (m)");
}

void loop() {
    // try to parse packet
    int packetSize = LoRa.parsePacket();
    if (packetSize) {
        // received a packet
        // Serial.print("Received packet '");
        
        byte UUID = LoRa.read(); // First byte is the Launchsonde UUID
        byte destUUID = LoRa.read(); // Second byte is the destination UUID
        byte packetType = LoRa.read(); // Third byte is the packet type

        // read packet
        char _buf[sizeof(data)+1];
        int i = 0;
        while (LoRa.available()) {
            char _byte = LoRa.read();
            _buf[i] = _byte;
            i++;
            // Serial.print(_byte, HEX); // DEBUG
        }
        memcpy(&data, _buf, sizeof(data)+1);

        printBaseStationTelemetry();
    }
}