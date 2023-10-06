// PWM output functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.


#include "board/gpio.h"
//TODO: PWM Code

struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    return (struct gpio_pwm){.reg = &pin};
}
void gpio_pwm_write(struct gpio_pwm g, uint32_t val)
{
}