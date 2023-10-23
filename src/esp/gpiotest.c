// GPIO functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "sched.h"
#include "board/gpio.h"
#include "command.h"
#include "driver/gpio.h"
#include "autoconf.h"
#if CONFIG_ESP32_EXP_IO_SPI
#include "driver/spi_master.h"
#elif CONFIG_ESP32_EXP_IO_I2S
#include "driver/i2s_common.h"
#include "driver/i2s_std.h"
#endif
#include "soc/gpio_struct.h"
#include "autoconf.h"
#include "internal.h"

#define ESP32_IO_MAX_NUM 40
#define ESP32_IO_BASE_B 0x100
#define ESP32_IO_BASE_C 0x200
#define ESP32_IO_PIN_MASK 0xff

#if CONFIG_ESP32_IO_EXPANSION // Pin Expansion With 74HCxx
#if CONFIG_ESP32_EXP_IO_SPI
#define ESP32_EXP_IO_SPI HSPI_HOST
#elif CONFIG_ESP32_EXP_IO_I2S
#define ESP32_I2S_FREQ (40 * 1000 * 1000) // 40MHz
#endif
#endif

#define PIN(x) (1ULL << x)
const uint64_t strapping_pin = PIN(0) | PIN(2) | PIN(5) | PIN(12) | PIN(15);
const uint64_t xtal_pin = 0;
const uint64_t spi_flash_pin = PIN(6) | PIN(7) | PIN(8) | PIN(9) | PIN(10) | PIN(11);
const uint64_t invalid_pins = xtal_pin | spi_flash_pin;
const uint64_t input_pins = PIN(34) | PIN(35) | PIN(36) | PIN(37) | PIN(38) | PIN(39);
gpio_config_t io_config;

DECL_ENUMERATION_RANGE("pin", "PA0", 0, ESP32_IO_MAX_NUM);
uint64_t io_pa_raw = 0;
#if CONFIG_ESP32_IO_EXPANSION
#if CONFIG_ESP32_EXP_IO_SPI
static spi_transaction_t trans = {
    .length = 64,
    .flags = 0,
    .rx_buffer = NULL,
};
spi_device_handle_t io_pb_handle;
volatile uint64_t io_pb_raw = 0;
#elif CONFIG_ESP32_EXP_IO_I2S
i2s_chan_handle_t io_pb_handle;
volatile uint32_t io_pb_raw = 0;
uint8_t io_pb_buff[4] = {0};

#endif
DECL_ENUMERATION_RANGE("pin", "PB0", ESP32_IO_BASE_B, CONFIG_ESP32_EXP_IO_COUNT);
uint8_t io_pb_inited = 0;

#if CONFIG_ESP32_IO_DUAL_EXPANSION
#if CONFIG_ESP32_EXP_IO_SPI
spi_device_handle_t io_pc_handle;
DECL_ENUMERATION_RANGE("pin", "PC0", ESP32_IO_BASE_C, CONFIG_ESP32_EXP2_IO_COUNT);
uint8_t io_pc_inited = 0;
volatile uint64_t io_pc_raw = 0;
#endif
#endif
#endif

#if CONFIG_ESP32_IO_EXPANSION
void gpio_exp_b_init(void)
{
    io_pb_inited = 1;
}

void gpio_exp_b_flush(void)
{
}
#endif

struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val)
{
    struct gpio_out ret = {
        .exp = 0,
        .bit = 0,
        .regs=NULL,
    };
    return ret;
}
// Reconfigure GPIO out pin
void gpio_out_reset(struct gpio_out g, uint32_t val)
{
}

// Toggle GPIO Pin(without interrupt)
void gpio_out_toggle_noirq(struct gpio_out g)
{
}

// Toggle GPIO PIN
void gpio_out_toggle(struct gpio_out g)
{
    // gpio_out_toggle_noirq(g);
}

#define HAL_FORCE_MODIFY_U32_REG_FIELD(base_reg, reg_field, field_val) \
    {                                                                  \
        uint32_t temp_val = base_reg.val;                              \
        typeof(base_reg) temp_reg;                                     \
        temp_reg.val = temp_val;                                       \
        temp_reg.reg_field = (field_val);                              \
        (base_reg).val = temp_reg.val;                                 \
    }

// write level to GPIO
void gpio_out_write(struct gpio_out g, uint32_t val)
{
}

// GPIO Input Functions
struct gpio_in gpio_in_setup(uint32_t pin, int32_t pull_up)
{
    struct gpio_in ret =
        {
            .bit = 0,
            .regs = &GPIO,
        };
    return ret;
}

void gpio_in_reset(struct gpio_in g, int32_t pull_up)
{
}

uint8_t gpio_in_read(struct gpio_in g)
{
    return 1;
}
#if CONFIG_ESP32_IO_EXPANSION
void gpio_init(void)
{
    io_pb_inited = 0;
    io_pb_raw = 0;
#if CONFIG_ESP32_IO_DUAL_EXPANSION
    io_pc_inited = 0;
    io_pc_raw = 0;
#endif
}
DECL_INIT(gpio_init);
#endif