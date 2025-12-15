#include "i2c_bus.h"

extern "C"
{
#include "nrfx_twi.h"
}

#define I2C_INSTANCE_ID 0

static const nrfx_twi_t m_twi = NRFX_TWI_INSTANCE(I2C_INSTANCE_ID);
static bool m_initialized = false;

bool i2c_init()
{
    if (m_initialized)
    {
        return true;
    }

    nrfx_twi_config_t config = {
        .scl = NRF_GPIO_PIN_MAP(0, 26),
        .sda = NRF_GPIO_PIN_MAP(0, 27),
        .frequency = NRF_TWI_FREQ_100K,
        .interrupt_priority = NRFX_TWI_DEFAULT_CONFIG_IRQ_PRIORITY,
        .hold_bus_uninit = false};

    nrfx_err_t err = nrfx_twi_init(&m_twi, &config, nullptr, nullptr);
    if (err != NRFX_SUCCESS)
    {
        return false;
    }

    nrfx_twi_enable(&m_twi);
    m_initialized = true;
    return true;
}

bool i2c_write(uint8_t address, const uint8_t *data, uint8_t length)
{
    if (!m_initialized)
    {
        return false;
    }

    nrfx_err_t err = nrfx_twi_tx(&m_twi, address, data, length, false);
    return (err == NRFX_SUCCESS);
}

bool i2c_read(uint8_t address, uint8_t *data, uint8_t length)
{
    if (!m_initialized)
    {
        return false;
    }

    nrfx_err_t err = nrfx_twi_rx(&m_twi, address, data, length);
    return (err == NRFX_SUCCESS);
}
