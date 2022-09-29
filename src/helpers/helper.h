#include <Arduino.h>

// Pin definitions
#include "pins.h"

// Telemetry helpers
#include "telemetry.h"

// Filesystem helpers
#include "filesystem.h"

static void printBaseStationTelemetry() {
    // Parse timestamp
    char _buf[64];
    sprintf(_buf, "%04d-%02d-%02dT%02d:%02d:%02d.%03d", data.year, data.month, data.day, data.hour, data.minute, data.second, data.msecond);

    Serial.print(_buf); Serial.print(',');
    Serial.print(data.voltage); Serial.print(',');
    Serial.print(data.GPSFix); Serial.print(',');
    Serial.print(data.numSats); Serial.print(',');
    Serial.print(data.HDOP); Serial.print(',');
    Serial.print(data.latitude); Serial.print(',');
    Serial.print(data.longitude); Serial.print(',');
    Serial.print(data.GPSSpeed); Serial.print(',');
    Serial.print(data.GPSCourse); Serial.print(',');
    Serial.print(data.baroTemp); Serial.print(',');
    Serial.print(data.pressure); Serial.print(',');
    Serial.print(data.altitude); Serial.print(',');
    Serial.print(data.sysCal); Serial.print(',');
    Serial.print(data.gyroCal); Serial.print(',');
    Serial.print(data.accelCal); Serial.print(',');
    Serial.print(data.magCal); Serial.print(',');
    Serial.print(data.accelX); Serial.print(',');
    Serial.print(data.accelY); Serial.print(',');
    Serial.print(data.accelZ); Serial.print(',');
    Serial.print(data.gyroX); Serial.print(',');
    Serial.print(data.gyroY); Serial.print(',');
    Serial.print(data.gyroZ); Serial.print(',');
    Serial.print(data.roll); Serial.print(',');
    Serial.print(data.pitch); Serial.print(',');
    Serial.print(data.yaw); Serial.print(',');
    Serial.print(data.linAccelX); Serial.print(',');
    Serial.print(data.linAccelY); Serial.print(',');
    Serial.print(data.linAccelZ); Serial.print(',');
    Serial.print(data.imuTemp); Serial.print(',');
    Serial.print(data.shtTemp); Serial.print(',');
    Serial.print(data.humidity); Serial.print(',');
    Serial.print(data.state); Serial.print(',');
    Serial.print(data.packetSize); Serial.print(',');
    Serial.print(0); Serial.print(','); // Period
    Serial.print(0); Serial.print(','); // Frequency
    Serial.print(0); Serial.print(','); // RSSI
    Serial.print(0); Serial.print(','); // DX
    Serial.print(0); Serial.print(','); // DY
    Serial.println(0);                  // Distance
}