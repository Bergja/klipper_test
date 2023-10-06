// ADC input functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h"

//TODO:  ADC Coding
struct gpio_adc gpio_adc_setup(uint32_t pin)
{
    return (struct gpio_adc){.chan = pin};
}
uint32_t gpio_adc_sample(struct gpio_adc g)
{
    return 0;
}
uint16_t gpio_adc_read(struct gpio_adc g)
{
    return 0;
}
void gpio_adc_cancel_sample(struct gpio_adc g)
{
}