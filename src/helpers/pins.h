#include <SPI.h>

#ifdef PROTOTYPE
    #define FSPI_SCLK_PIN   37
    #define FSPI_MISO_PIN   36
    #define FSPI_MOSI_PIN   35
    #define FSPI_CS_PIN     38
    SPIClass LoRaSPI(FSPI);

    #define RFM_CS_PIN 38
    #define RFM_RST_PIN 0
    #define RFM_IRQ_PIN 33

#endif