// ADC input functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h"
#include "command.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "sched.h"
#include "board/irq.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"
#include "internal.h"

#define PIN(x) (1ULL << x)
const uint64_t adc_pin = PIN(32) | PIN(33) | PIN(34) | PIN(35) | PIN(36) | PIN(37) | PIN(38) | PIN(39);
adc_oneshot_unit_handle_t adc_handle;
adc_cali_handle_t adc_cali;
volatile uint8_t adc_inited;
QueueHandle_t adc_request_queue;
volatile int adc_val[16] = {0};
int adc_lastval[16] = {0};
volatile uint8_t adc_ready = 0;
volatile uint8_t adc_quested = 0;

DECL_CONSTANT("ADC_MAX", 4095);

void gpio_adc_init(void)
{
}
DECL_INIT(gpio_adc_init);

struct gpio_adc gpio_adc_setup(uint32_t pin)
{
    struct gpio_adc ret = {.adc = NULL};
    return ret;
}

uint32_t gpio_adc_sample(struct gpio_adc g)
{
    return 0;
}

uint16_t gpio_adc_read(struct gpio_adc g)
{
    return 0xff;
}

void gpio_adc_cancel_sample(struct gpio_adc g)
{
}
