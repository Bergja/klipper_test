// Main starting point for ESP32
//
// Copyright (C) 2023  Zhuangzhe <chelieo@aol.com>

// This file may be distributed under the terms of the GNU GPLv3 license.

#include "sched.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

volatile uint8_t esp_irq_stat=1;

static const char *TAG = "klipper main";

void prebuilt_func(void)
{
    sched_main();
    vTaskDelete(NULL);
}

void shut_down_func(void)
{
    ESP_LOGI(TAG,"SYSTEM Shutdown");
}
DECL_SHUTDOWN(shut_down_func);