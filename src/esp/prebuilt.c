// Main starting point for ESP32
//
// Copyright (C) 2023  Zhuangzhe <chelieo@aol.com>

// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sched.h"
#include "internal.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "autoconf.h"
#include "command.h"

volatile uint8_t esp_irq_stat = 1;

static const char *TAG = "klipper main";

DECL_CONSTANT_STR("MCU", CONFIG_MCU);
#if CONFIG_DEBUG
#include "gpio.h"

void test_loop(void)
{
    gpio_adc_init();
    struct gpio_adc adc1 = gpio_adc_setup(39);
    uint16_t adc_val;
    while (1)
    {
        while (gpio_adc_sample(adc1))
        {
            vTaskDelay(10);
        }
        adc_val = gpio_adc_read(adc1);
        DEBUGI(TAG, "ADC:%d", adc_val);
    }
}

#endif

void prebuilt_func(void)
{
#if CONFIG_DEBUG
    test_loop();
#else
    sched_main();
#endif
    vTaskDelete(NULL);
}

void shut_down_func(void)
{
    DEBUGI(TAG, "SYSTEM Shutdown");
}
DECL_SHUTDOWN(shut_down_func);