// Serial Interface for ESP32
//
// Copyright (C) 2023  Zhuangzhe <chelieo@aol.com>

// This file may be distributed under the terms of the GNU GPLv3 license.
#include <string.h> // memmove
#include <fcntl.h>  // fcntl

#include "autoconf.h"   // CONFIG_SERIAL_BAUD
#include "board/io.h"   // readb
#include "board/irq.h"  // irq_save
#include "board/misc.h" // console_sendf
#include "board/pgm.h"  // READP
#include "command.h"    // DECL_CONSTANT
#include "sched.h"      // sched_wake_tasks

#include "board/serial_irq.h" // serial_get_tx_byte
#include "sched.h"            // DECL_INIT
#include "internal.h"
#include "driver/uart.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "freertos/portmacro.h"

#define RX_BUFFER_SIZE 192
#define RX_ESP_BUFFER_SIZE RX_BUFFER_SIZE + 32
#define TX_BUFFER_SIZE 192
#define UART_QUEUE_SIZE 0

#if CONFIG_KLIPPER_SERIAL_U0
#define KLIPPER_SERIAL UART_NUM_0
#elif CONFIG_KLIPPER_SERIAL_U1
#define KLIPPER_SERIAL UART_NUM_1
#elif CONFIG_KLIPPER_SERIAL_U2
#define KLIPPER_SERIAL UART_NUM_2
#endif

static const char *TAG = "klipper serial";

uint8_t klipper_serial_rx_buff[RX_ESP_BUFFER_SIZE];
static uint8_t klipper_serial_rx_pos = 0;
uint8_t klipper_serial_tx_buff[TX_BUFFER_SIZE];

DECL_CONSTANT("SERIAL_BAUD", CONFIG_SERIAL_BAUD);
DECL_CONSTANT("RECEIVE_WINDOW", RX_BUFFER_SIZE);
static QueueHandle_t klipper_serial_queue;

static inline void serial_rx(void)
{
    int len, maxlen;
    maxlen = RX_ESP_BUFFER_SIZE - klipper_serial_rx_pos;
    if (maxlen > RX_BUFFER_SIZE)
    {
        maxlen = RX_BUFFER_SIZE;
    }
    else if (!maxlen)
    {
        klipper_serial_rx_pos = 0;
        maxlen = RX_BUFFER_SIZE;
    }

    ESP_ERROR_CHECK(uart_get_buffered_data_len(KLIPPER_SERIAL, (size_t *)&len));
    if (len > maxlen)
    {
        len = maxlen;
    }
    len = uart_read_bytes(KLIPPER_SERIAL, klipper_serial_rx_buff + klipper_serial_rx_pos, len, 0);
    if (len > 0)
    {
        for (maxlen = 0; maxlen < len; maxlen++)
        {
            if (*(klipper_serial_rx_buff + klipper_serial_rx_pos + maxlen) == MESSAGE_SYNC)
            {
                sched_wake_tasks();
            }
        }
        klipper_serial_rx_pos += len;
        // DEBUGI(TAG, "%X", *(klipper_serial_rx_buff + klipper_serial_rx_pos - 1));
    }
}

// Serial Event Handle Process
static void serial_event_task(void *pvParameters)
{
    uart_event_t event;
    DEBUGI(TAG, "TaskStart");
    for (;;)
    {
        // DEBUGI(TAG, "W");
        // Waiting for UART event.
        if (xQueueReceive(klipper_serial_queue, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            // DEBUGI(TAG, "E");
            switch (event.type)
            {
            // Event of UART receving data
            /*We'd better handler data event fast, there would be much more data events than
            other types of events. If we take too much time on data event, the queue might
            be full.*/
            case UART_FIFO_OVF:
            case UART_BUFFER_FULL:
            case UART_PATTERN_DET:
            case UART_BREAK:
            case UART_DATA:
                serial_rx();
                break;
                break;
            // Event of UART parity check error
            case UART_PARITY_ERR:
                DEBUGI(TAG, "uart parity error");
                break;
            // Event of UART frame error
            case UART_FRAME_ERR:
                DEBUGI(TAG, "uart frame error");
                break;
            // Others
            default:
                DEBUGI(TAG, "uart event type: %d", event.type);
                break;
            }
        }
    }
    vTaskDelete(NULL);
}

void serial_init(void)
{
    uart_config_t esp_serial_config = {
        .baud_rate = CONFIG_SERIAL_BAUD,
        .data_bits = UART_DATA_8_BITS,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .parity = UART_PARITY_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
        .stop_bits = UART_STOP_BITS_1,
        .rx_flow_ctrl_thresh = 122};
    ESP_ERROR_CHECK(uart_param_config(KLIPPER_SERIAL, &esp_serial_config));
    DEBUGI(TAG, "UART_Configured");
    ESP_ERROR_CHECK(uart_set_pin(KLIPPER_SERIAL, CONFIG_KLIPPER_SERIAL_TXD_PIN, CONFIG_KLIPPER_SERIAL_RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
    DEBUGI(TAG, "UART_Pin_Set");
    ESP_ERROR_CHECK(uart_driver_install(KLIPPER_SERIAL, 2048, TX_BUFFER_SIZE, 5, &klipper_serial_queue, ESP_INTR_FLAG_LEVEL1));
    ESP_ERROR_CHECK(uart_enable_rx_intr(KLIPPER_SERIAL));
    ESP_ERROR_CHECK(uart_set_rx_full_threshold(KLIPPER_SERIAL, 100));
    xTaskCreatePinnedToCore(&serial_event_task, "serial_event_task", 2048, NULL, 2, NULL, 1);
    uint32_t temp;
    uart_get_baudrate(KLIPPER_SERIAL, &temp);
    DEBUGI("klipperUart", "Rate:%ld", temp);
    // // Make stdin/stdout non-blocking
    // fcntl(STDIN_FILENO, F_SETFL, fcntl(STDIN_FILENO, F_GETFL, 0) | O_NONBLOCK);
    // fcntl(STDOUT_FILENO, F_SETFL
    //       , fcntl(STDOUT_FILENO, F_GETFL, 0) | O_NONBLOCK);
}
DECL_INIT(serial_init);

// Encode and transmit response
void console_sendf(const struct command_encoder *ce, va_list args)
{
    const char *tx_buff = (char *)&klipper_serial_tx_buff[0];
    uint_fast8_t max_size = READP(ce->max_size);
    if (max_size > TX_BUFFER_SIZE)
        return; // Not enough space for message

    uint_fast8_t msglen = command_encode_and_frame(klipper_serial_tx_buff, ce, args);

    uart_tx_chars(KLIPPER_SERIAL, tx_buff, msglen);
    // uart_wait_tx_done(KLIPPER_SERIAL,100);
}

static inline void
console_pop_input(uint_fast8_t len)
{
    __irq_disable();
    uint_fast8_t rpos = readb(&klipper_serial_rx_pos);
    uint_fast8_t needcopy = rpos - len;
    if (needcopy)
    {
        memmove(&klipper_serial_rx_buff[0], &klipper_serial_rx_buff[len], needcopy);
    }
    klipper_serial_rx_pos = needcopy;
    __irq_enable();
    sched_wake_tasks();
}

// Process Income Commands
void console_task(void)
{
    uint_fast8_t rpos = readb(&klipper_serial_rx_pos), pop_count;
    int_fast8_t ret = command_find_block(klipper_serial_rx_buff, rpos, &pop_count);
    if (ret > 0)
        command_dispatch(klipper_serial_rx_buff, pop_count);
    if (ret)
    {
        // DEBUGI(TAG, "Run%d", ret);
        if (CONFIG_HAVE_BOOTLOADER_REQUEST && ret < 0 && pop_count == 32 && !memcmp(klipper_serial_rx_buff, " \x1c Request Serial Bootloader!! ~", 32))
        {
            bootloader_request();
        }
        console_pop_input(pop_count);
        if (ret > 0)
        {
            command_send_ack();
        }
        if (ret < 0)
        {
            // DEBUGI("klipper", "free%d", RX_BUFFER_SIZE - klipper_serial_rx_pos);
        }
    }
    // DEBUGI("klipper", "cmd%d", ret);
}
DECL_TASK(console_task);