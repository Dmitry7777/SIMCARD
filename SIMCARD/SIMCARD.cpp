#include "SIMCARD.h"

// Constructor for a single SIM card
SIMCARD::SIMCARD(uint8_t SIM_RST, uint8_t SIM_CLK, uint8_t SIM_VPP, uint8_t SIM_IO, uint8_t SIM_VCC) {
    _SIM_RST0 = SIM_RST;
    _SIM_CLK0 = SIM_CLK;
    _SIM_VPP0 = SIM_VPP;
    _SIM_IO0 = SIM_IO;
    _SIM_VCC0 = SIM_VCC;
}

// Constructor for dual SIM card setup
SIMCARD::SIMCARD(uint8_t SIM_RST0, uint8_t SIM_CLK0, uint8_t SIM_VPP0, uint8_t SIM_IO0, uint8_t SIM_VCC0,
                 uint8_t SIM_RST1, uint8_t SIM_CLK1, uint8_t SIM_VPP1, uint8_t SIM_IO1, uint8_t SIM_VCC1) {
    _SIM_RST0 = SIM_RST0;
    _SIM_CLK0 = SIM_CLK0;
    _SIM_VPP0 = SIM_VPP0;
    _SIM_IO0 = SIM_IO0;
    _SIM_VCC0 = SIM_VCC0;

    _SIM_RST1 = SIM_RST1;
    _SIM_CLK1 = SIM_CLK1;
    _SIM_VPP1 = SIM_VPP1;
    _SIM_IO1 = SIM_IO1;
    _SIM_VCC1 = SIM_VCC1;
}

// Initialize SIM card
void SIMCARD::initSIM(uint8_t simNum) {
    uint8_t SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC;
    selectSIM(simNum, SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC);

    pinMode(SIM_RST, OUTPUT);
    pinMode(SIM_CLK, OUTPUT);
    pinMode(SIM_VPP, OUTPUT);
    pinMode(SIM_IO, INPUT);   // Set as input for communication
    pinMode(SIM_VCC, OUTPUT);
}

// Power on the SIM card
void SIMCARD::powerOn(uint8_t simNum) {
    uint8_t SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC;
    selectSIM(simNum, SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC);

    digitalWrite(SIM_VCC, HIGH);  // Power up the SIM card
    digitalWrite(SIM_RST, HIGH);  // Reset to high
}

// Power off the SIM card
void SIMCARD::powerOff(uint8_t simNum) {
    uint8_t SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC;
    selectSIM(simNum, SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC);

    digitalWrite(SIM_VCC, LOW);  // Power down the SIM card
}

// Send data to SIM card
void SIMCARD::sendData(uint8_t simNum, const uint8_t *data, uint16_t length) {
    for (uint16_t i = 0; i < length; i++) {
        sendByte(simNum, data[i]);
    }
}

// Receive data from SIM card
uint16_t SIMCARD::receiveData(uint8_t simNum, uint8_t *buffer, uint16_t bufferSize) {
    uint16_t bytesRead = 0;
    for (uint16_t i = 0; i < bufferSize; i++) {
        buffer[i] = readByte(simNum);
        bytesRead++;
    }
    return bytesRead;
}

// Helper function to send a byte of data
void SIMCARD::sendByte(uint8_t simNum, uint8_t data) {
    uint8_t SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC;
    selectSIM(simNum, SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC);

    for (int i = 7; i >= 0; i--) {
        digitalWrite(SIM_CLK, LOW);
        digitalWrite(SIM_IO, (data & (1 << i)) ? HIGH : LOW);
        delayMicroseconds(1);
        digitalWrite(SIM_CLK, HIGH);
        delayMicroseconds(1);
    }
}

// Helper function to read a byte of data
uint8_t SIMCARD::readByte(uint8_t simNum) {
    uint8_t SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC;
    selectSIM(simNum, SIM_RST, SIM_CLK, SIM_VPP, SIM_IO, SIM_VCC);

    uint8_t data = 0;
    for (int i = 7; i >= 0; i--) {
        digitalWrite(SIM_CLK, LOW);
        delayMicroseconds(1);
        if (digitalRead(SIM_IO)) {
            data |= (1 << i);
        }
        digitalWrite(SIM_CLK, HIGH);
        delayMicroseconds(1);
    }
    return data;
}

// Select the SIM pins based on the SIM number (0 or 1)
void SIMCARD::selectSIM(uint8_t simNum, uint8_t &SIM_RST, uint8_t &SIM_CLK, uint8_t &SIM_VPP, uint8_t &SIM_IO, uint8_t &SIM_VCC) {
    if (simNum == 0) {
        SIM_RST = _SIM_RST0;
        SIM_CLK = _SIM_CLK0;
        SIM_VPP = _SIM_VPP0;
        SIM_IO = _SIM_IO0;
        SIM_VCC = _SIM_VCC0;
    } else if (simNum == 1) {
        SIM_RST = _SIM_RST1;
        SIM_CLK = _SIM_CLK1;
        SIM_VPP = _SIM_VPP1;
        SIM_IO = _SIM_IO1;
        SIM_VCC = _SIM_VCC1;
    }
}