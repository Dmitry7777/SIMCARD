#ifndef SIMCARD_H
#define SIMCARD_H

#include <Arduino.h>

class SIMCARD {
public:
    // Constructor for a single SIM card connection
    SIMCARD(uint8_t SIM_RST, uint8_t SIM_CLK, uint8_t SIM_VPP, uint8_t SIM_IO, uint8_t SIM_VCC);

    // Constructor for dual SIM card setup
    SIMCARD(uint8_t SIM_RST0, uint8_t SIM_CLK0, uint8_t SIM_VPP0, uint8_t SIM_IO0, uint8_t SIM_VCC0,
            uint8_t SIM_RST1, uint8_t SIM_CLK1, uint8_t SIM_VPP1, uint8_t SIM_IO1, uint8_t SIM_VCC1);

    // Initialize SIM card (set the pins to proper mode)
    void initSIM(uint8_t simNum = 0);

    // Power on the SIM card
    void powerOn(uint8_t simNum = 0);

    // Power off the SIM card
    void powerOff(uint8_t simNum = 0);

    // Send data to SIM card
    void sendData(uint8_t simNum, const uint8_t *data, uint16_t length);

    // Receive data from SIM card
    uint16_t receiveData(uint8_t simNum, uint8_t *buffer, uint16_t bufferSize);

private:
    // Pin variables for the first SIM card
    uint8_t _SIM_RST0, _SIM_CLK0, _SIM_VPP0, _SIM_IO0, _SIM_VCC0;

    // Pin variables for the second SIM card (optional)
    uint8_t _SIM_RST1, _SIM_CLK1, _SIM_VPP1, _SIM_IO1, _SIM_VCC1;

    // Helper functions for handling individual SIM cards
    void sendByte(uint8_t simNum, uint8_t data);
    uint8_t readByte(uint8_t simNum);

    // Select the correct pins for a specific SIM card
    void selectSIM(uint8_t simNum, uint8_t &SIM_RST, uint8_t &SIM_CLK, uint8_t &SIM_VPP, uint8_t &SIM_IO, uint8_t &SIM_VCC);
};

#endif