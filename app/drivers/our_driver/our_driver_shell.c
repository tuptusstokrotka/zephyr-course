#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

static const struct device *our_driver_dev = DEVICE_DT_GET(DT_NODELABEL(our_driver0));

static int sample_fetch_driver_handler(const struct shell *shell, size_t argc, char **argv)
{
    struct led_task_data *data = (struct led_task_data *)our_driver_dev->data;
    data->blinking_on = false;

    sensor_sample_fetch(our_driver_dev);
    return 0;
}

static int channel_get_driver_handler(const struct shell *shell, size_t argc, char **argv)
{
    struct led_task_data *data = (struct    led_task_data *)our_driver_dev->data;
    data->blinking_on = false;

    struct sensor_value val;
    sensor_channel_get(our_driver_dev, SENSOR_CHAN_AMBIENT_TEMP, &val);
    shell_print(shell, "LED turned OFF. Channel value: %d; %d", val.val1, val.val2);
    return 0;
}

static int device_info_handler(const struct shell *shell, size_t argc, char **argv)
{
    shell_print(shell, "Our Driver\n\tDevice name: %s\n\tInit: %s\n\tStatus: %d", our_driver_dev->name, our_driver_dev->state->initialized ? "Initialized" : "Not Initialized", our_driver_dev->state->init_res);
    return 0;
}

static int device_blink_handler(const struct shell *shell, size_t argc, char **argv)
{
    const uint32_t lower_bound = 100, upper_bound = 2000;
    uint32_t blink_count = 0;
    blink_count = strtoul(argv[1], NULL, 10);

    if (blink_count < lower_bound || blink_count > upper_bound) {
        shell_error(shell, "Invalid blink count. Please enter a value between %d and %d.", lower_bound, upper_bound);
        return -EINVAL;
    }

    struct led_task_data *data = (struct led_task_data *)our_driver_dev->data;
    data->blinking_on = true;

    our_driver_set_sleep_ms(our_driver_dev, blink_count);
    return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sub_our_driver,
    SHELL_CMD(fetch, NULL, "Get data of our driver (turn ON LED)", sample_fetch_driver_handler),
    SHELL_CMD(read, NULL, "Read data of our driver (turn OFF LED)", channel_get_driver_handler),
    SHELL_CMD(info, NULL, "Print device name and status", device_info_handler),
    SHELL_CMD_ARG(set, NULL, "Set blinking led period [ms]", device_blink_handler, 2, 0),
    SHELL_SUBCMD_SET_END,
);

SHELL_CMD_REGISTER(sensor, &sub_our_driver, "Commands to test our driver", NULL);