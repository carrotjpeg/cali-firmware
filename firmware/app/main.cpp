#include "../drivers/common/i2c_bus.h"
#include "../drivers/common/ble_log.h"
#include "../drivers/tmp117/tmp117.h"

int main() {
    i2c_init();
    ble_log_init();

    TMP117 temp;
    float temp_c = 0.0f;

    while (1) {
        if (temp.readTemperatureC(temp_c)) {
            ble_log_send("TMP117_C", temp_c);
        }
    }
}
