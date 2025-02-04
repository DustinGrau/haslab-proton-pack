#include <Arduino.h>

// Define SPI pins
#define PIN_MOSI 23  
#define PIN_MISO 19  
#define PIN_SCLK 18  
#define PIN_CS   5   

// Enum for SPI modes
enum SPIMode {
    MODE0, // CPOL = 0, CPHA = 0 (Clock idle LOW, data captured on rising edge, output on falling edge)
    MODE1, // CPOL = 0, CPHA = 1 (Clock idle LOW, data captured on falling edge, output on rising edge)
    MODE2, // CPOL = 1, CPHA = 0 (Clock idle HIGH, data captured on falling edge, output on rising edge)
    MODE3  // CPOL = 1, CPHA = 1 (Clock idle HIGH, data captured on rising edge, output on falling edge)
};

SPIMode currentMode = MODE0; // Default SPI mode

// SPI monitoring variables
volatile uint8_t byteBufferMOSI = 0;
volatile uint8_t byteBufferMISO = 0;
volatile uint8_t bitCount = 0;
volatile bool newByteAvailable = false;
volatile bool csActive = false;
volatile uint32_t byteCount = 0; // Tracks number of bytes in a transaction

// Interrupt on SPI clock signal
void IRAM_ATTR onClockEdge() {
    bool clockRising = digitalRead(PIN_SCLK); // True if clock transitions from LOW to HIGH

    // Determine data capture based on SPI mode
    bool captureData = false;
    switch (currentMode) {
        case MODE0: captureData = clockRising; break;  // Capture on rising edge
        case MODE1: captureData = !clockRising; break; // Capture on falling edge
        case MODE2: captureData = !clockRising; break; // Capture on falling edge
        case MODE3: captureData = clockRising; break;  // Capture on rising edge
    }

    if (captureData) {
        byteBufferMOSI <<= 1;
        byteBufferMISO <<= 1;

        if (digitalRead(PIN_MOSI)) byteBufferMOSI |= 1;
        if (digitalRead(PIN_MISO)) byteBufferMISO |= 1;
    }
    
    bitCount++;  
    if (bitCount == 8) {
        bitCount = 0;
        newByteAvailable = true;
        byteCount++; // Increment byte count during an active transaction
    }
}

// Interrupt on CS pin to detect transaction start and end
void IRAM_ATTR onCSEdge() {
    csActive = !digitalRead(PIN_CS); // LOW means active, HIGH means inactive
    if (csActive) {
        byteCount = 0; // Reset byte counter at start of transaction
    }
}

// Process and send SPI data with format options
enum OUT_Format { OUT_HEX, OUT_DECIMAL, OUT_ASCII };
void processSPIData(OUT_Format format = OUT_HEX) {
    if (newByteAvailable) {
        String message = "";

        if (csActive) {
            message += "--> ";

            switch (format) {
                case OUT_HEX:
                    message += "M.Send: 0x" + String(byteBufferMOSI, HEX) + " | S.Resp: 0x" + String(byteBufferMISO, HEX);
                    break;
                case OUT_DECIMAL:
                    message += "M.Send: " + String(byteBufferMOSI) + " | S.Resp: " + String(byteBufferMISO);
                    break;
                case OUT_ASCII:
                    message += "M.Send: '" + String((char)byteBufferMOSI) + "' | S.Resp: '" + String((char)byteBufferMISO) + "'";
                    break;
            }

            Serial.println(message);
        } else {
            Serial.println("===");
        }

        newByteAvailable = false;
    }
}

void setup() {
    Serial.begin(115200);
    pinMode(PIN_MOSI, INPUT);
    pinMode(PIN_MISO, INPUT);
    pinMode(PIN_SCLK, INPUT);
    pinMode(PIN_CS, INPUT);

    attachInterrupt(digitalPinToInterrupt(PIN_SCLK), onClockEdge, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_CS), onCSEdge, CHANGE);

    Serial.println("SPI Monitor Initialized.");
}

void loop() {
    processSPIData(OUT_HEX); // Default to HEX output; change to OUT_DECIMAL or OUT_ASCII if needed
}
