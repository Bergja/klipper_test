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
volatile QueueHandle_t adc_request_queue;
volatile int adc_val[16] = {0};
volatile int adc_lastval[16] = {0};
volatile uint8_t adc_ready = 0;
volatile uint8_t adc_quested = 0;
const adc_channel_t adc_chn[] = {
    ADC_CHANNEL_0,
    ADC_CHANNEL_1,
    ADC_CHANNEL_2,
    ADC_CHANNEL_3,
    ADC_CHANNEL_4,
    ADC_CHANNEL_5,
    ADC_CHANNEL_6,
    ADC_CHANNEL_7,
};

DECL_CONSTANT("ADC_MAX", 4095);

static void adc_event_task(void *pvParameters)
{
    static adc_channel_t chan;
    int ret;
    for (;;)
    {
        if (xQueueReceive(adc_request_queue, &chan, (TickType_t)portMAX_DELAY))
        {
            // DEBUGI("KlipperADC", "chn;%d", chan);
            if (adc_inited & PIN(chan))
            {
                ESP_ERROR_CHECK(adc_oneshot_get_calibrated_result(adc_handle, adc_cali, chan, &ret));
                adc_val[chan] = ret;
                adc_ready |= PIN(chan);
                // DEBUGI("KlipperADC", "chn:%lld,val;%d", PIN(chan),adc_val[chan]);
            }
        }
    }
    vTaskDelete(NULL);
}

static bool gpio_adc_calibration_init(adc_unit_t unit, adc_atten_t atten, adc_cali_handle_t *out_handle)
{
    adc_cali_handle_t handle = NULL;
    esp_err_t ret = ESP_FAIL;
    bool calibrated = false;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    if (!calibrated)
    {
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }
#endif

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    if (!calibrated)
    {
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
        if (ret == ESP_OK)
        {
            calibrated = true;
        }
    }
#endif

    *out_handle = handle;
    return calibrated;
}

void gpio_adc_init(void)
{
    DEBUGI("klipper ADC", "Initfunc");
    adc_oneshot_unit_init_cfg_t init_cfg = {
        .clk_src = ADC_DIGI_CLK_SRC_DEFAULT,
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_cfg, &adc_handle));
    adc_inited = 0;
    adc_ready = 0;
    adc_quested = 0;
    adc_request_queue = xQueueCreate(16, sizeof(adc_channel_t));
    xTaskCreatePinnedToCore(&adc_event_task, "ADC Task", 2048, NULL, 4, NULL, 1);
}
DECL_INIT(gpio_adc_init);

struct gpio_adc gpio_adc_setup(uint32_t pin)
{
    struct gpio_adc ret = {.adc = NULL};
    if (adc_pin & PIN(pin))
    {
        switch (pin)
        {
        case 32:
            ret.chan = ADC_CHANNEL_4;
            break;
        case 33:
            ret.chan = ADC_CHANNEL_5;
            break;
        case 34:
            ret.chan = ADC_CHANNEL_6;
            break;
        case 35:
            ret.chan = ADC_CHANNEL_7;
            break;
        case 36:
            ret.chan = ADC_CHANNEL_0;
            break;
        case 37:
            ret.chan = ADC_CHANNEL_1;
            break;
        case 38:
            ret.chan = ADC_CHANNEL_2;
            break;
        case 39:
            ret.chan = ADC_CHANNEL_3;
            break;
        default:
            ret.chan = 0XFF;
            break;
        }
        if (ret.chan != 0xff)
        {
            ret.adc = &adc_handle;
            adc_oneshot_chan_cfg_t adc_chn_cfg = {
                .bitwidth = ADC_BITWIDTH_DEFAULT,
#if CONFIG_ESP_ADC_RG_FULL
                .atten = ADC_ATTEN_DB_11,
#elif CONFIG_ESP_ADC_RG_2V2
                .atten = ADC_ATTEN_DB_6,
#endif
            };
            ESP_ERROR_CHECK(adc_oneshot_config_channel(*(ret.adc), ret.chan, &adc_chn_cfg));
            if (!adc_inited)
            {
                adc_inited = PIN(ret.chan);
                if (!gpio_adc_calibration_init(ADC_UNIT_1, adc_chn_cfg.atten, &adc_cali))
                {
                    ESP_ERROR_CHECK(ESP_FAIL);
                }
            }
            else
            {
                adc_inited |= PIN(ret.chan);
            }
            DEBUGI("klipperADC:", "Pin%ld", pin);
        }
    }
    else
    {
        shutdown("Invalid ADC pin");
    }
    return ret;
}

uint32_t gpio_adc_sample(struct gpio_adc g)
{
    if (adc_inited & PIN(g.chan))
    {
        if (adc_quested & PIN(g.chan))
        {
            if (adc_ready & PIN(g.chan))
            {
                return 0;
            }
            return 50;
        }
        else
        {
            adc_ready &= !PIN(g.chan);
            adc_quested |= PIN(g.chan);
            xQueueSend(adc_request_queue, (void *)&g.chan, 0);
            return 70;
        }
    }
    DEBUGI("klipperADC", "ready:%x,quested:%x", adc_ready, adc_quested);
    return 60;
}

uint16_t gpio_adc_read(struct gpio_adc g)
{
    uint16_t temp = 0;

    if (g.chan < 8 && (adc_inited & PIN(g.chan)))
    {
        if (!(adc_quested & PIN(g.chan)))
        {
            adc_ready &= !PIN(g.chan);
            adc_quested |= PIN(g.chan);
            xQueueSend(adc_request_queue, (void *)&g.chan, 0);
            temp = (double)adc_lastval[g.chan] * 4095.0 / 2180.0;
        }
        else
        {
            if ((adc_ready & PIN(g.chan)))
            {
                temp = (double)adc_val[g.chan] * 4095.0 / 2180.0;
                adc_lastval[g.chan] = adc_val[g.chan];
                adc_quested &= !PIN(g.chan);
            }
            else
            {
                temp = (double)adc_lastval[g.chan] * 4095.0 / 2180.0;
                DEBUGI("klipperADCNoT", "ready:%x,quested:%x", adc_ready, adc_quested);
            }
        }
    }
    return temp;
}

void gpio_adc_cancel_sample(struct gpio_adc g)
{
    if (adc_inited & PIN(g.chan))
    {
        if (adc_ready & PIN(g.chan))
        {
            adc_ready &= !PIN(g.chan);
        }
    }
}
