#include "../drivers/tmp117/tmp117.h"
#include "../drivers/common/i2c_bus.h"

int main()
{
    i2c_init();

    TMP117 temp;

    int16_t raw_temp = 0;

    while (1)
    {
        temp.readRaw(raw_temp);
    }
}
