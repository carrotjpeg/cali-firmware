#pragma once

#include <stdint.h>

class TMP117
{
public:
    explicit TMP117(uint8_t i2c_address = 0x48);

    bool init();
    bool readRaw(int16_t &raw_temp);

private:
    uint8_t address;
};
