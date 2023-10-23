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
#include "freertos/queue.h"
#include "freertos/task.h"
#include "freertos/projdefs.h"
#include "irq.h"

#define ESP_TIMER_QUEUE_MAX 10
static const char *TAG = "klipper timer";
// Global Timer Handle
gptimer_handle_t hgptimer = NULL;
gptimer_alarm_config_t agptimer;
QueueHandle_t timer_dispatch_queue;
volatile uint8_t timer_requested = 0;
uint8_t tempret;

// Activate timer dispatch as soon as possible
IRAM_ATTR void
timer_kick(void)
{
    uint64_t current;
    __irq_disable();
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer, &current));
    agptimer.alarm_count = current + 100;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer, &agptimer));
    __irq_enable();
}
void timer_dispatch_task(void *pvParameters)
{
    uint8_t ret;
    uint64_t current;
    uint32_t next = 100;
    uint32_t ccurrent;
    uint8_t max_timer_queue=0;
    for (;;)
    {
        if (xQueueReceive(timer_dispatch_queue, (void *)&ret, (TickType_t)portMAX_DELAY))
        {
            if(timer_requested>max_timer_queue){
                max_timer_queue=timer_requested;
                if(max_timer_queue>1){
                    DEBUGI("klipperTimer","MaxQue%d",max_timer_queue);
                }
            }
            if (timer_requested)
            {
                ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer, &current));
                next = timer_dispatch_many();
                ccurrent = current;
                if (next > ccurrent)
                {
                    current += next - ccurrent;
                }
                else
                {
                    current += (uint64_t)next - ccurrent + 0x100000000;
                }
                agptimer.alarm_count = current;
                ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer, &agptimer));
            }
        }
        timer_requested = 0;
    }
    DEBUGI("TimerTask", "Exit");
    vTaskDelay(10);
    vTaskDelete(NULL);
}
IRAM_ATTR static bool timer_isr(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    if (timer_requested<ESP_TIMER_QUEUE_MAX)
    {
        timer_requested++;
        xQueueSendFromISR(timer_dispatch_queue, &tempret, pdFALSE);
    }
    return pdTRUE;
}
void timer_init(void)
{
    timer_dispatch_queue = xQueueCreate(1, 1);
    timer_requested = 0;
    DEBUGI(TAG, "Creating Timer");
    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = CONFIG_CLOCK_FREQ};
    ESP_ERROR_CHECK(gptimer_new_timer(&timer_config, &hgptimer));
    DEBUGI(TAG, "Starting Timer");
    agptimer.alarm_count = CONFIG_CLOCK_FREQ; // 1s alarm to ensure not start now
    agptimer.reload_count = 0;
    agptimer.flags.auto_reload_on_alarm = false;
    ESP_ERROR_CHECK(gptimer_set_alarm_action(hgptimer, &agptimer));
    gptimer_event_callbacks_t gptimer_cb = {.on_alarm = timer_isr};
    ESP_ERROR_CHECK(gptimer_register_event_callbacks(hgptimer, &gptimer_cb, NULL));
    ESP_ERROR_CHECK(gptimer_enable(hgptimer));
    ESP_ERROR_CHECK(gptimer_start(hgptimer));
    ESP_ERROR_CHECK(gptimer_set_raw_count(hgptimer, 0));
    timer_kick();
    xTaskCreatePinnedToCore(&timer_dispatch_task, "Timer Dispatch Task", 10240, NULL, 2, NULL, 1);
}
DECL_INIT(timer_init);

// Retrun Current Time
IRAM_ATTR uint32_t
timer_read_time(void)
{
    uint64_t cnt;
    uint32_t ret;
    ESP_ERROR_CHECK(gptimer_get_raw_count(hgptimer, &cnt));
    ret = cnt;
    return ret;
}
