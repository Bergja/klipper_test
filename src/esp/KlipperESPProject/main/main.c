#include <stdio.h>
#include "prebuilt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

void app_main(void)
{
    // 等待APP Core 初始化完成
    vTaskDelay(pdMS_TO_TICKS(100));
    // 创建Klipper任务于CPU1(APP)
    xTaskCreatePinnedToCore(&prebuilt_func, "Klipper Task", 10240, NULL, 1, NULL, 1);
}
