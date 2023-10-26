#pragma once

#include "esp_log.h"
#include "autoconf.h"

#if CONFIG_DEBUG
#define DEBUGI( tag, format, ... ) ESP_LOGI(tag,format,##__VA_ARGS__)
#else
#define DEBUGI( tag, format, ... ) //tag,format,##__VA_ARGS__
#endif