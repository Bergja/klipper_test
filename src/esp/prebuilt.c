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
#include "esp_timer.h"

void test_loop(void)
{
    uint64_t tim1,tim2,tim3;
    gpio_adc_init();
    struct gpio_adc adc1 = gpio_adc_setup(39);
    struct gpio_adc adc2 = gpio_adc_setup(36);
    uint16_t adc_val,adc_val2;
    vTaskPrioritySet(NULL,2);
    while (1)
    {
        tim1=esp_timer_get_time();
        while (gpio_adc_sample(adc1))
        {
            taskYIELD();
        }
        tim2=esp_timer_get_time();
        adc_val = gpio_adc_read(adc1);
        tim3=esp_timer_get_time();
        while (gpio_adc_sample(adc2))
        {
            taskYIELD();
        }
        adc_val2 = gpio_adc_read(adc2);
        DEBUGI(TAG, "ADC:%d 2:%d,TIM:T1:%lldT2:%lld", adc_val,adc_val2,(tim2-tim1),(tim3-tim2));
    }
}

#endif

void prebuilt_func(void)
{
#if CONFIG_DEBUG
    // test_loop();
    sched_main();
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