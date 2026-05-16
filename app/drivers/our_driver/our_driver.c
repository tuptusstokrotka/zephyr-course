#include "zephyr/device.h"
#include "zephyr/drivers/sensor.h"
#include "zephyr/logging/log.h"

#define DT_DRV_COMPAT our_driver

LOG_MODULE_REGISTER(our_driver, LOG_LEVEL_INF);

static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val){
    LOG_INF("Our Driver channel %d get called", chan);
    return 0;
}

static DEVICE_API(sensor, api_iomico) = {
    .channel_get = our_driver_channel_get,
};

static int init(const struct device *dev){
    LOG_INF("Our Driver init called");
    return 0;
}

DEVICE_DT_INST_DEFINE(0,            // Instance number
                      init,         // Initialization
                      NULL,         // Power management
                      NULL,         // Mutable data
                      NULL,         // Init config
                      POST_KERNEL,  // Initialization level
                      80,           // Initialization priority
                      &api_iomico   // API structure
                    );
