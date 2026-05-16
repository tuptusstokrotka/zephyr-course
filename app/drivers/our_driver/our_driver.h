#pragma once

#include <zephyr/device.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

int our_driver_set_sleep_ms(const struct device *dev, uint32_t sleep_ms);

int our_driver_get_sleep_ms(const struct device *dev);


#ifdef __cplusplus
}
#endif