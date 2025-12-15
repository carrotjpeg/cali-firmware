#pragma once

#include <stdint.h>
#include <stdbool.h>

bool i2c_init();
bool i2c_write(uint8_t address, const uint8_t* data, uint8_t length);
bool i2c_read(uint8_t address, uint8_t* data, uint8_t length);
