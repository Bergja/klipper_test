// PWM output functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h"
#include "driver/ledc.h"
#include "sched.h"
#include "command.h"
#include "autoconf.h"
#define ESP_LEDC_TIMER_MAX_NUM 4
#define ESP_LEDC_CHANNEL_MAX_NUM 8
uint8_t pwm_io[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t pwm_init_num = 0;
uint32_t pwm_clk_list[ESP_LEDC_TIMER_MAX_NUM] = {0};
uint8_t pwm_clk_num = 0;
void gpio_pwm_init(void)
{
    pwm_init_num = 0;
    pwm_clk_num = 0;
}
DECL_INIT(gpio_pwm_init);

struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val)
{
    struct gpio_pwm ret = {.chan=0xff};
    ledc_channel_config_t pwm_cfg;
    uint8_t i;
    for (i = 0; i < pwm_clk_num; i++)
    {
        if (pwm_clk_list[i] == cycle_time)
        {
            break;
        }
    }
    if (i != pwm_clk_num)
    {
        pwm_cfg.timer_sel = i;
    }
    else if (pwm_clk_num < 4)
    {
        ledc_timer_config_t pwm_tim;
        double freq = (CONFIG_CLOCK_FREQ * 256 / cycle_time);
        uint8_t res;
        pwm_tim.duty_resolution = LEDC_TIMER_8_BIT;
        pwm_tim.speed_mode = LEDC_HIGH_SPEED_MODE;
        if (freq > 1e6)
        {
            pwm_tim.clk_cfg = LEDC_USE_APB_CLK; // 80MHz
            freq = 8e7 / freq;
            res = 0;
            while (freq > 1)
            {
                res++;
                freq /= 2;
            }
            pwm_tim.freq_hz = (80 * 1000 * 1000) >> res;
        }
        else
        {
            pwm_tim.clk_cfg = LEDC_USE_REF_TICK; // 1MHz
            freq = 1e6 / freq;
            res = 0;
            while (freq > 1)
            {
                res++;
                freq /= 2;
            }
            if (res > 12)
            {
                return ret;
            }
            pwm_tim.freq_hz = (1 * 1000 * 1000) >> res;
        }
        pwm_tim.timer_num = pwm_clk_num;
        pwm_clk_list[pwm_clk_num] = cycle_time;
        pwm_cfg.timer_sel = pwm_clk_num;
        pwm_clk_num++;
        ESP_ERROR_CHECK(ledc_timer_config(&pwm_tim));
    }
    else
    {
        shutdown("PWM Hardware Exceeds");
        return ret;
    }
    if (pin < 39 && i < ESP_LEDC_CHANNEL_MAX_NUM)
    {
        pwm_cfg.duty = val;
        pwm_cfg.flags.output_invert = false;
        pwm_cfg.gpio_num = pin;
        pwm_cfg.hpoint = 0;
        pwm_cfg.intr_type = LEDC_INTR_DISABLE;
        pwm_cfg.speed_mode = LEDC_HIGH_SPEED_MODE;
        for (i=0; i < ESP_LEDC_CHANNEL_MAX_NUM; i++)
        {
            if (pwm_io[i] == 0xff)
            {
                pwm_io[i] = pin;
                pwm_cfg.channel = i;
                break;
            }
            else if (pwm_io[i] == pin)
            {
                pwm_cfg.channel = i;
                break;
            }
        }
        if (i < ESP_LEDC_CHANNEL_MAX_NUM)
        {
            ESP_ERROR_CHECK(ledc_channel_config(&pwm_cfg));
            ret.chan=pwm_cfg.channel;
        }
        else
        {
            shutdown("PWM Hardware Exceeds");
            return ret;
        }
    }
    return ret;
}

void gpio_pwm_write(struct gpio_pwm g, uint32_t val)
{
    if(g.chan<ESP_LEDC_CHANNEL_MAX_NUM){
        ESP_ERROR_CHECK(ledc_set_duty(LEDC_HIGH_SPEED_MODE,g.chan,val));
        ESP_ERROR_CHECK(ledc_update_duty(LEDC_HIGH_SPEED_MODE,g.chan));
    }
}