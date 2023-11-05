// hardware I2C functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "autoconf.h"
#if CONFIG_HAVE_GPIO_I2C

#include "board/gpio.h"
#include "driver/i2c.h"
#include "sched.h"
#include "command.h"

#define BUS(x) (1ULL << x)

#if CONFIG_ESP32_I2C_0 || CONFIG_ESP32_I2C_1
i2c_port_t i2c_bus[] = {
#if CONFIG_ESP32_I2C_0
    I2C_NUM_0,
#endif
#if CONFIG_ESP32_I2C_1
    I2C_NUM_1,
#endif
};
typedef struct
{
    uint8_t scl;
    uint8_t sda;
} i2c_pinout;
i2c_pinout i2c_pins[] = {
#if CONFIG_ESP32_I2C_0
    {.scl = CONFIG_ESP32_I2C0_SCL, .sda = CONFIG_ESP32_I2C0_SDA},
#endif
#if CONFIG_ESP32_I2C_1
    {.scl = CONFIG_ESP32_I2C1_SCL, .sda = CONFIG_ESP32_I2C1_SDA},
#endif
};
#endif
uint8_t i2c_bus_inited = 0;
void i2c_init(void)
{
    i2c_bus_inited = 0;
}
DECL_INIT(i2c_init);

struct i2c_config i2c_setup(uint32_t bus, uint32_t rate, uint8_t addr)
{
    struct i2c_config ret = {.addr = 0, .i2c = 0xff};
    if (bus < sizeof(i2c_bus))
    {
        ret.i2c = i2c_bus[bus];
        ret.addr = addr;
        if (!(BUS(bus) & i2c_bus_inited))
        {
            // shutdown("i2c bus inited already");
        }
        else
        {
            i2c_config_t i2c_cfg = {
                .mode = I2C_MODE_MASTER,
                .sda_io_num = i2c_pins[bus].sda,
                .sda_pullup_en = GPIO_PULLUP_ENABLE,
                .scl_io_num = i2c_pins[bus].scl,
                .scl_pullup_en = GPIO_PULLUP_ENABLE,
                .master.clk_speed = rate,
                .clk_flags = I2C_SCLK_SRC_FLAG_FOR_NOMAL,
            };
            ESP_ERROR_CHECK(i2c_param_config(ret.i2c, &i2c_cfg));
            ESP_ERROR_CHECK(i2c_driver_install(ret.i2c, I2C_MODE_MASTER, 0, 0, ESP_INTR_FLAG_IRAM));
        }
    }
    else
    {
        shutdown("i2c hardware exceeded!");
    }
    return ret;
}
void i2c_write(struct i2c_config config, uint8_t write_len, uint8_t *write)
{
    const uint8_t *buff;
    buff = write;
    if (config.i2c != 0xff)
    {
        esp_err_t err = ESP_OK;
        i2c_cmd_handle_t cmd = i2c_cmd_link_create();
        err = i2c_master_start(cmd);
        if (err != ESP_OK)
        {
            goto end;
        }
        err = i2c_master_write_byte(cmd, config.addr << 1 | I2C_MASTER_WRITE, true);
        if (err != ESP_OK)
        {
            goto end;
        }
        err = i2c_master_write(cmd, buff, write_len, true);
        if (err != ESP_OK)
        {
            goto end;
        }
        err = i2c_master_stop(cmd);
        if (err != ESP_OK)
        {
            goto end;
        }
        err = i2c_master_cmd_begin(config.i2c, cmd, 10);
    end:
        i2c_cmd_link_delete(cmd);
    }
}
void i2c_read(struct i2c_config config, uint8_t reg_len, uint8_t *reg, uint8_t read_len, uint8_t *read)
{
    if(config.i2c!=0xff){
        esp_err_t err=ESP_OK;
        i2c_cmd_handle_t cmd=i2c_cmd_link_create();
        err=i2c_master_start(cmd);
        if (err != ESP_OK)
        {
            goto end;
        }
        err=i2c_master_write_byte(cmd,config.addr<<1|I2C_MASTER_READ,true);
        if (err != ESP_OK)
        {
            goto end;
        }
        err=i2c_master_read(cmd,read,read_len,I2C_MASTER_LAST_NACK);
        if (err != ESP_OK)
        {
            goto end;
        }
        err=i2c_master_stop(cmd);
        if (err != ESP_OK)
        {
            goto end;
        }
        err=i2c_master_cmd_begin(config.i2c,cmd,10);
    end:
        i2c_cmd_link_delete(cmd);
    }
}
#endif