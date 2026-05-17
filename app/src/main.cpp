#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include "our_driver.h"

#define LED_NODE DT_ALIAS(app_led) // Add flag: -DEXTRA_DTC_OVERLAY_FILE="boards/app.overlay"
#define OUR_DRIVER_NODE DT_NODELABEL(our_driver0)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct device *our_driver_dev = DEVICE_DT_GET(OUR_DRIVER_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

namespace{
    void api_test(){
        struct sensor_value val;
        auto channel = sensor_channel_get(our_driver_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
        auto sample = sensor_sample_fetch(our_driver_dev);

        LOG_INF("API test called. Channel: %d. Sample fetched %d, Value: %d; %d", channel, sample, val.val1, val.val2);
    }
}

int main(void)
{
    api_test();
    if (!device_is_ready(our_driver_dev)) return -1;

    struct sensor_value val;

    our_driver_set_sleep_ms(our_driver_dev, CONFIG_APP_HEARTBEAT_PERIOD_MS);

    while (1) {
        // sensor_sample_fetch(our_driver_dev);
        // sensor_channel_get(our_driver_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
        k_msleep(100); // Avoid empty loop
    }
    return 0;
}
