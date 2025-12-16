#include "../drivers/common/i2c_bus.h"
#include "../drivers/common/uart_log.h"
#include "../drivers/tmp117/tmp117.h"

int main() {
    i2c_init();
    uart_log_init();

    TMP117 temp;
    float temp_c = 0.0f;

    while (1) {
        if (temp.readTemperatureC(temp_c)) {
            uart_log_printf("TMP117: %.2f C\n", temp_c);
        }
    }
}
