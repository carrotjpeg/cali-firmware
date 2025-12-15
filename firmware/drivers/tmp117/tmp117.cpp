#include "tmp117.h"
#include "../common/i2c_bus.h"

#define TMP117_TEMP_REG 0x00

TMP117::TMP117(uint8_t i2c_address)
    : address(i2c_address) {}

bool TMP117::init()
{
    // TMP117 does not require special init for basic reads
    return true;
}

bool TMP117::readRaw(int16_t &raw_temp)
{
    uint8_t reg = TMP117_TEMP_REG;
    uint8_t data[2] = {0};

    if (!i2c_write(address, &reg, 1))
    {
        return false;
    }

    if (!i2c_read(address, data, 2))
    {
        return false;
    }

    raw_temp = (data[0] << 8) | data[1];
    return true;
}
