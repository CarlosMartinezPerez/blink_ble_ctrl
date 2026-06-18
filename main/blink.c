#include "blink.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#define BLINK_GPIO GPIO_NUM_18

static volatile uint32_t blink_period_ms = 1000;

static void blink_task(void *pvParameters)
{
    bool led_state = false;

    while (1)
    {
        led_state = !led_state;
        gpio_set_level(BLINK_GPIO, led_state);

        vTaskDelay(pdMS_TO_TICKS(blink_period_ms));
    }
}

void blink_init(void)
{
    gpio_reset_pin(BLINK_GPIO);
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT);

    xTaskCreate(
        blink_task,
        "blink_task",
        2048,
        NULL,
        5,
        NULL);
}

void blink_set_period(uint32_t period_ms)
{
    if (period_ms > 0)
    {
        blink_period_ms = period_ms;
    }
}