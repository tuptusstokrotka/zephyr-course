#include "zephyr/device.h"
#include "zephyr/devicetree.h"
#include "zephyr/drivers/sensor.h"
#include "zephyr/logging/log.h"
#include <zephyr/drivers/gpio.h>

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

#define DT_DRV_COMPAT our_driver

struct led_task_cfg {
    struct gpio_dt_spec led;
    uint32_t sleep_ms;
};

static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val){
    const struct led_task_cfg *led = dev->config;
    if (gpio_pin_set_dt(&led->led, 0) < 0) return -EIO;
    LOG_INF("Our Driver channel get called for channel %d", chan);
    k_msleep(led->sleep_ms);
    return 0;
}

static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan){
    const struct led_task_cfg *led = dev->config;
    if (gpio_pin_set_dt(&led->led, 1) < 0) return -EIO;
    LOG_INF("Our Driver sample fetch called for channel %d", chan);
    k_msleep(led->sleep_ms);
    return 0;
}

static DEVICE_API(sensor, api_iomico) = {
    .channel_get = our_driver_channel_get,
    .sample_fetch = our_driver_sample_fetch,
};

static int init(const struct device *dev){
    const struct led_task_cfg *led = dev->config;
    if (!gpio_is_ready_dt(&led->led)) return -ENODEV;
    if (gpio_pin_configure_dt(&led->led, GPIO_OUTPUT_ACTIVE) < 0) return -EIO;
    LOG_INF("Our Driver init called");
    return 0;
}

// https://docs.zephyrproject.org/latest/build/dts/howtos.html#write-device-drivers-using-devicetree-apis
#define DEVICE_INST(inst)                                           \
    static const struct led_task_cfg my_cfg_##inst = {              \
        .led        = GPIO_DT_SPEC_INST_GET(inst, gpios),           \
        .sleep_ms   = DT_INST_PROP(inst, sleep_ms),                 \
    };                                                              \
    DEVICE_DT_INST_DEFINE(inst,                                     \
                        init,                                       \
                        NULL,                                       \
                        NULL,                                       \
                        &my_cfg_##inst,                             \
                        POST_KERNEL,                                \
                        80,                                         \
                        &api_iomico);

/*
    DEVICE_DT_INST_DEFINE(inst,          // Instance number
                          init,          // Initialization
                          NULL,          // Power management
                          NULL,          // Mutable data
                          NULL,          // Init config
                          POST_KERNEL,   // Initialization level
                          80,            // Initialization priority
                          &api_iomico    // API structure
                        );
*/

DT_INST_FOREACH_STATUS_OKAY(DEVICE_INST);
