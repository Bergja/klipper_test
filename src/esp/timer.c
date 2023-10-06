// Timer Interface for ESP32
//
// Copyright (C) 2023  Zhuangzhe <chelieo@aol.com>

// This file may be distributed under the terms of the GNU GPLv3 license.

#include <time.h>
#include "driver/gptimer.h"
#include "autoconf.h"
#include "esp_log.h"
#include "generic/timer_irq.h"
#include "sched.h"
#include "freertos/FreeRTOS.h"
#include "freertos/projdefs.h"
#include "irq.h"

static const char *TAG = "klipper timer";
//Global Timer Handle
gptimer_handle_t hgptimer=NULL; 
gptimer_alarm_config_t agptimer;

//Activate timer dispatch as soon as possible
IRAM_ATTR void
timer_kick(void){
    uint64_t current;
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer,&current));
    agptimer.alarm_count=current+100;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer,&agptimer));
}

IRAM_ATTR static bool timer_isr(gptimer_handle_t timer,const gptimer_alarm_event_data_t *edata,void *user_ctx)
{
    uint64_t current;
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer,&current));
    uint32_t next = timer_dispatch_many();
    uint32_t ccurrent=current;
    if(next>ccurrent){
        current+=next-ccurrent;
    }
    else{
        current+=(uint64_t)next-ccurrent+0x100000000;
    }
    irq_disable();
    agptimer.alarm_count=current;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer,&agptimer));
    irq_enable();
    return pdTRUE;
}
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
    agptimer.alarm_count=CONFIG_CLOCK_FREQ;//1s alarm to ensure not start now
    agptimer.reload_count=0;
    agptimer.flags.auto_reload_on_alarm=false;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer,&agptimer));
    gptimer_event_callbacks_t cbs={
        .on_alarm=timer_isr
    };
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(hgptimer,&cbs,NULL));
    ESP_ERROR_CHECK(gptimer_enable(hgptimer));
    ESP_ERROR_CHECK(gptimer_start(hgptimer));
    ESP_ERROR_CHECK(gptimer_set_raw_count(hgptimer,0));
    ESP_LOGI(TAG,"Timer started!");
    timer_kick();
}
DECL_INIT(timer_init);

//Retrun Current Time
IRAM_ATTR uint32_t
timer_read_time(void)
{
    uint64_t cnt;
    uint32_t ret;
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer,&cnt));
    ret=cnt;
    return ret;
}


