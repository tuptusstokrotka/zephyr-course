#pragma once

#include <zephyr/device.h>
#include <zephyr/drivers/gpio.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

struct led_task_cfg {
    struct gpio_dt_spec led;
};

struct led_task_data {
    uint32_t sleep_ms;
    bool blinking_on; // true if use blining, false if just on/off via shell
};

int our_driver_set_sleep_ms(const struct device *dev, uint32_t sleep_ms);

int our_driver_get_sleep_ms(const struct device *dev);


#ifdef __cplusplus
}
#endif