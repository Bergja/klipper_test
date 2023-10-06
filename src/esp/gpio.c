// GPIO functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "sched.h"
#include "board/gpio.h"
#include "command.h"
#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "autoconf.h"
#include "esp_log.h"

#define ESP32_IO_MAX_NUM 39
#define ESP32_IO_BASE_B 0x100
#define ESP32_IO_BASE_C 0x200
#define ESP32_IO_PIN_MASK 0xff

#if CONFIG_ESP32_IO_EXPANSION // Pin Expansion With 74HCxx
#define ESP32_EXP_IO_SPI HSPI_HOST
#endif

#define PIN(x) (1ULL << x)
const uint64_t strapping_pin = PIN(0) | PIN(2) | PIN(5) | PIN(12) | PIN(15);
const uint64_t xtal_pin = 0;
const uint64_t spi_flash_pin = PIN(6) | PIN(7) | PIN(8) | PIN(9) | PIN(10) | PIN(11);
const uint64_t invalid_pins = strapping_pin | xtal_pin | spi_flash_pin;
const uint64_t input_pins = PIN(34) | PIN(35) | PIN(36) | PIN(37) | PIN(38) | PIN(39);
gpio_config_t io_config;

DECL_ENUMERATION_RANGE("pin", "PA0", 0, ESP32_IO_MAX_NUM);
uint64_t io_pa_raw = 0;
#if CONFIG_ESP32_IO_EXPANSION
static spi_transaction_t trans = {
    .length = 64,
    .flags = 0,
    .rx_buffer = NULL,
    };
DECL_ENUMERATION_RANGE("pin", "PB0", ESP32_IO_BASE_B, CONFIG_ESP32_EXP_IO_COUNT);
spi_device_handle_t io_pb_handle;
uint8_t io_pb_inited = 0;
volatile uint64_t io_pb_raw = 0;

#if CONFIG_ESP32_IO_DUAL_EXPANSION
DECL_ENUMERATION_RANGE("pin", "PC0", ESP32_IO_BASE_C, CONFIG_ESP32_EXP2_IO_COUNT);
spi_device_handle_t io_pc_handle;
uint8_t io_pc_inited = 0;
volatile uint64_t io_pc_raw = 0;
#endif
#endif

#if CONFIG_ESP32_IO_EXPANSION
void gpio_exp_b_init(void)
{
    spi_bus_config_t busspi = {
        .mosi_io_num = CONFIG_ESP32_EXP_IO_DATA_PIN,
        .miso_io_num = -1,
        // .data0_io_num = -1,
        // .data1_io_num = -1,
        // .data2_io_num = -1,
        // .data3_io_num = -1,
        // .data4_io_num = -1,
        // .data5_io_num = -1,
        // .data6_io_num = -1,
        // .data7_io_num = -1,
        .sclk_io_num = CONFIG_ESP32_EXP_IO_RCLK_PIN,
        .quadhd_io_num = -1,
        .quadwp_io_num = -1,
        .max_transfer_sz = 1024,
        .isr_cpu_id = 1,
    };
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_26M,
        .mode = 0,
        .address_bits = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .command_bits = 0,
        .dummy_bits = 0,
        .queue_size = 1,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 1,
        .spics_io_num = CONFIG_ESP32_EXP_IO_SCLK_PIN,
        .flags = 0,
    };
    ESP_ERROR_CHECK(spi_bus_initialize(ESP32_EXP_IO_SPI, &busspi, SPI_DMA_CH_AUTO));
    ESP_ERROR_CHECK(spi_bus_add_device(ESP32_EXP_IO_SPI, &devcfg, &io_pb_handle));
    io_pb_inited=1;
}

void gpio_exp_b_flush(void)
{
    uint64_t temp=io_pb_raw;
    trans.tx_buffer = &temp;
    ESP_ERROR_CHECK(spi_device_queue_trans(io_pb_handle, &trans, 10));
    ESP_LOGI("GPIO","val:%lld",io_pb_raw);
}
#if CONFIG_ESP32_IO_DUAL_EXPANSION
void gpio_exp_c_init(void)
{
    if (!io_pb_inited)
    {
        gpio_exp_b_init();
    }
    spi_device_interface_config_t devcfg = {
        .clock_speed_hz = SPI_MASTER_FREQ_26M,
        .mode = 0,
        .address_bits = 0,
        .clock_source = SPI_CLK_SRC_DEFAULT,
        .command_bits = 0,
        .dummy_bits = 0,
        .queue_size = 1,
        .cs_ena_pretrans = 0,
        .cs_ena_posttrans = 1,
        .spics_io_num = CONFIG_ESP32_EXP2_IO_SCLK_PIN,
        .flags = 0,
    };
    ESP_ERROR_CHECK(spi_bus_add_device(ESP32_EXP_IO_SPI, &devcfg, &io_pb_handle));
    io_pc_inited=1;
}

void gpio_exp_c_flush(void)
{
    uint64_t temp=io_pc_raw;
    trans.tx_buffer = &temp;
    ESP_ERROR_CHECK(spi_device_queue_trans(io_pc_handle, &trans, 10));
}
#endif
#endif

struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val)
{
    uint32_t real_pin;
    struct gpio_out ret = {
        .exp = 0,
        .bit = 0,
    };
    real_pin = pin & ESP32_IO_PIN_MASK;

#if CONFIG_ESP32_IO_EXPANSION
    if (pin & ESP32_IO_BASE_B && real_pin < CONFIG_ESP32_EXP_IO_COUNT)
    {
        if (!io_pb_inited)
        {
            gpio_exp_b_init();
        }
        ret.exp = 1;
        ret.bit = PIN(real_pin);
        gpio_out_write(ret, val);
    }
    else
    {
#if CONFIG_ESP32_IO_DUAL_EXPANSION
        if (pin & ESP32_IO_BASE_C && real_pin < CONFIG_ESP32_EXP2_IO_COUNT)
        {
            if (!io_pc_inited)
            {
                gpio_exp_c_init();
            }
            ret.exp = 2;
            ret.bit = PIN(real_pin);
            gpio_out_write(ret, val);
        }
        else
        {
#endif
#endif
            if (pin == real_pin && real_pin < ESP32_IO_MAX_NUM)
            {
                if (PIN(real_pin) & invalid_pins & input_pins)
                {
                    shutdown("INVALID IO PIN");
                }
                else
                {
                    io_config.intr_type = GPIO_INTR_DISABLE;
                    io_config.mode = GPIO_MODE_OUTPUT;
                    io_config.pin_bit_mask = PIN(real_pin);
                    io_config.pull_down_en = 0;
                    io_config.pull_up_en = 0;
                    ESP_ERROR_CHECK(gpio_config(&io_config));
                    ret.exp = 0;
                    ret.bit = PIN(real_pin);
                    ret.regs = &GPIO;
                    gpio_out_write(ret, val);
                }
            }
            else
            {
                shutdown("Unknown PIN");
            }
#if CONFIG_ESP32_IO_EXPANSION
        }
#if CONFIG_ESP32_IO_DUAL_EXPANSION
    }
#endif
#endif
    return ret;
}
// Reconfigure GPIO out pin
void gpio_out_reset(struct gpio_out g, uint32_t val)
{
    if (g.exp == 0)
    {
        io_config.intr_type = GPIO_INTR_DISABLE;
        io_config.mode = GPIO_MODE_OUTPUT;
        io_config.pin_bit_mask = g.bit;
        io_config.pull_down_en = 0;
        io_config.pull_up_en = 0;
        ESP_ERROR_CHECK(gpio_config(&io_config));
    }
    gpio_out_write(g, val);
}

// Toggle GPIO Pin(without interrupt)
void gpio_out_toggle_noirq(struct gpio_out g)
{
#if CONFIG_ESP32_IO_EXPANSION
    switch (g.exp)
    {
    case 0:
        if (io_pa_raw & g.bit)
        {
            gpio_out_write(g, 0);
        }
        else
        {
            gpio_out_write(g, 1);
        }
        break;
    case 1:
        if (io_pb_raw & g.bit)
        {
            gpio_out_write(g, 0);
        }
        else
        {
            gpio_out_write(g, 1);
        }
        break;
#if CONFIG_ESP32_IO_DUAL_EXPANSION
    case 2:
        if (io_pc_raw & g.bit)
        {
            gpio_out_write(g, 0);
        }
        else
        {
            gpio_out_write(g, 1);
        }
        break;
#endif
    default:
        break;
    }
#else
    if (io_pa_raw & g.bit)
    {
        gpio_out_write(g, 0);
    }
    else
    {
        gpio_out_write(g, 1);
    }
#endif
}

// Toggle GPIO PIN
void gpio_out_toggle(struct gpio_out g)
{
    gpio_out_toggle_noirq(g);
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
    gpio_dev_t *hw = g.regs;
#if CONFIG_ESP32_IO_EXPANSION
    switch (g.exp)
    {
    case 0:
        if (val)
        {
            if (g.bit < 0x100000000)
            {
                hw->out_w1ts = g.bit;
            }
            else
            {
                HAL_FORCE_MODIFY_U32_REG_FIELD(hw->out1_w1ts, data, g.bit >> 32);
            }
            io_pa_raw |= g.bit;
        }
        else
        {
            if (g.bit < 0x100000000)
            {
                hw->out_w1tc = g.bit;
            }
            else
            {
                HAL_FORCE_MODIFY_U32_REG_FIELD(hw->out1_w1tc, data, g.bit >> 32);
            }
            io_pa_raw &= !g.bit;
        }
        break;
    case 1:
        if (val)
        {
            io_pb_raw |= g.bit;
            gpio_exp_b_flush();
        }
        else
        {
            io_pb_raw &= !g.bit;
            gpio_exp_b_flush();
        }
        break;
#if CONFIG_ESP32_IO_DUAL_EXPANSION
    case 2:
        if (val)
        {
            io_pc_raw |= g.bit;
            gpio_exp_c_flush();
        }
        else
        {
            io_pc_raw &= !g.bit;
            gpio_exp_c_flush();
        }
        break;
#endif

    default:
        break;
    }
#else
    if (val)
    {
        if (g.bit < 0x100000000)
        {
            hw->out_w1ts = g.bit;
        }
        else
        {
            HAL_FORCE_MODIFY_U32_REG_FIELD(hw->out1_w1ts, data, g.bit >> 32);
        }
        io_pa_raw |= g.bit;
    }
    else
    {
        if (g.bit < 0x100000000)
        {
            hw->out_w1tc = g.bit;
        }
        else
        {
            HAL_FORCE_MODIFY_U32_REG_FIELD(hw->out1_w1tc, data, g.bit >> 32);
        }
        io_pa_raw &= !g.bit;
    }
#endif
}

// GPIO Input Functions
struct gpio_in gpio_in_setup(uint32_t pin, int32_t pull_up)
{
    uint32_t real_pin;
    struct gpio_in ret =
        {
            .bit = 0,
            .regs = &GPIO,
        };
    real_pin = pin & ESP32_IO_PIN_MASK;
    if (pin == real_pin && real_pin < ESP32_IO_MAX_NUM)
    {
        if (PIN(real_pin) & invalid_pins)
        {
            shutdown("INVALID IO PIN");
        }
        else
        {
            io_config.intr_type = GPIO_INTR_DISABLE;
            io_config.mode = GPIO_MODE_INPUT;
            io_config.pin_bit_mask = PIN(real_pin);
            if (pull_up)
            {
                io_config.pull_down_en = 0;
                io_config.pull_up_en = true;
            }
            else
            {
                io_config.pull_down_en = 0;
                io_config.pull_up_en = 0;
            }
            ESP_ERROR_CHECK(gpio_config(&io_config));
            ret.bit = PIN(real_pin);
            ret.regs = &GPIO;
        }
    }
    else
    {
        shutdown("Unkown PIN");
    }
    return ret;
}

void gpio_in_reset(struct gpio_in g, int32_t pull_up)
{
    io_config.intr_type = GPIO_INTR_DISABLE;
    io_config.mode = GPIO_MODE_INPUT;
    io_config.pin_bit_mask = g.bit;
    if (pull_up)
    {
        io_config.pull_down_en = 0;
        io_config.pull_up_en = true;
    }
    else
    {
        io_config.pull_down_en = 0;
        io_config.pull_up_en = 0;
    }
    ESP_ERROR_CHECK(gpio_config(&io_config));
}

uint8_t gpio_in_read(struct gpio_in g)
{
    gpio_dev_t *hw = g.regs;

    if (g.bit < 0x100000000)
    {
        return ((g.bit & hw->in) & true);
    }
    else
    {
        return (((g.bit >> 32) & hw->in1.data) && true);
    }
}

// TODO:  should be moved to other file



