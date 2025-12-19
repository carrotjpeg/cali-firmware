#include "../drivers/common/i2c_bus.h"
#include "../drivers/common/ble_stack.h"
#include "../drivers/common/ble_log.h"
#include "../drivers/tmp117/tmp117.h"
#include "nrf_delay.h"

int main(void)
{
    i2c_init();

    ble_stack_init_and_start(); // SoftDevice + GAP + advertising
    ble_log_init();             // registers custom service

    TMP117 temp(0x48);
    temp.init();

    float temp_c = 0.0f;

    while (1)
    {
        if (temp.readTemperatureC(temp_c))
        {
            ble_log_send("TMP117_C", temp_c);
        }

        sd_app_evt_wait(); // let SoftDevice run
    }
}
