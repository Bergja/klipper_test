// Timer Interface for ESP32
//
// Copyright (C) 2023  Zhuangzhe <chelieo@aol.com>

// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h>
#include "driver/gptimer.h"
#include "autoconf.h"
#include "esp_log.h"
#include "sched.h"

static const char *TAG = "klipper timer";
//Global Timer Handle
gptimer_handle_t hgptimer=NULL; 
void
timer_init(void)
{
    ESP_LOGI(TAG,"Creating Timer");
    gptimer_config_t timer_config={
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction=GPTIMER_COUNT_UP,
        .resolution_hz=CONFIG_CLOCK_FREQ
    };
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config,&hgptimer));
    // gptimer_event_callbacks_t cbs = {
    //     .on_alarm = klipper_timer_callback,
    // };
    // ESP_ERROR_CHECK(gptimer_register_event_callbacks(gptimer, &cbs, NULL));
    ESP_LOGI(TAG,"Starting Timer");
    ESP_ERROR_CHECK(gptimer_enable(hgptimer));
    ESP_ERROR_CHECK(gptimer_start(hgptimer));
    ESP_LOGI(TAG,"Timer started!");
    timer_kick();
}
DECL_INIT(timer_init);

uint32_t
timer_read_time(void)
{
    uint64_t cnt;
    uint32_t ret;
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer,&cnt));
    ret=cnt;
    return ret;
}
void
timer_kick(void){
    gptimer_set_raw_count(hgptimer,0);
}