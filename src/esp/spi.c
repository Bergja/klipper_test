// Hardware SPI functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h"
#include "autoconf.h"
#include "sched.h"
#include "command.h"
#include "driver/spi_master.h"
#include "string.h"

#if CONFIG_ESP32_SPI_ENABLE && (CONFIG_ESP_SPI_HSPI || CONFIG_ESP_SPI_VSPI)
#define BUS(x) (1ULL << x)
spi_host_device_t spi_bus[] = {
#if CONFIG_ESP_SPI_HSPI
    HSPI_HOST,
#endif
#if CONFIG_ESP_SPI_VSPI
    VSPI_HOST,
#endif
};

typedef struct
{
    int mosi, miso, sclk;
} spi_pinout;

spi_pinout spi_pins[] = {
#if CONFIG_ESP_SPI_HSPI
    {
        .mosi = CONFIG_ESP_HSPI_MOSI,
        .miso = CONFIG_ESP_HSPI_MISO,
        .sclk = CONFIG_ESP_HSPI_CLK,
    },
#endif
#if CONFIG_ESP_SPI_VSPI
    {.mosi = CONFIG_ESP_VSPI_MOSI,
     .miso = CONFIG_ESP_VSPI_MISO,
     .sclk = CONFIG_ESP_VSPI_CLK，}，
#endif
};
uint8_t rx_buffer[128];

spi_transaction_t trans = {
    .flags = 0,
    .rx_buffer = rx_buffer,
};

spi_device_handle_t spi_handle[2];
uint8_t spi_inited;
#endif

struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    struct spi_config ret = {.spi = NULL};
    if (rate > SPI_MASTER_FREQ_26M)
    {
        shutdown("SPI rate too high");
        return ret;
    }
    if (bus < sizeof(spi_bus))
    {
        if (BUS(bus) & spi_inited)
        {
            ESP_ERROR_CHECK(spi_bus_remove_device(spi_handle[bus]));
            ESP_ERROR_CHECK(spi_bus_free(spi_bus[bus]));
        }
        else
        {
            spi_bus_config_t busspi = {
                // .data0_io_num=-1,
                // .data0_io_num=-1,
                // .data2_io_num=-1,
                // .data3_io_num=-1,
                // .data4_io_num=-1,
                // .data5_io_num=-1,
                // .data6_io_num=-1,
                // .data7_io_num=-1,
                .mosi_io_num = spi_pins[bus].mosi,
                .miso_io_num = spi_pins[bus].miso,
                .sclk_io_num = spi_pins[bus].sclk,
                .quadhd_io_num = -1,
                .quadwp_io_num = -1,
                .max_transfer_sz = 1024,
                .isr_cpu_id = 1,
            };
            ESP_ERROR_CHECK(spi_bus_initialize(spi_bus[bus], &busspi, SPI_DMA_CH_AUTO));
        }
        spi_device_interface_config_t devcfg = {
            .clock_speed_hz = rate,
            .mode = mode,
            .address_bits = 0,
            .clock_source = SPI_CLK_SRC_DEFAULT,
            .command_bits = 0,
            .dummy_bits = 0,
            .queue_size = 1,
            .cs_ena_posttrans = 1,
            .cs_ena_pretrans = 1,
            .spics_io_num = -1,
            .flags = 0,
        };
        ESP_ERROR_CHECK(spi_bus_add_device(spi_bus[bus], &devcfg, &spi_handle[bus]));
        ret.spi = &spi_handle[bus];
    }
    return ret;
}
inline void spi_prepare(struct spi_config config)
{
    // Do Nothing
}

void spi_transfer(struct spi_config config, uint8_t receive_data, uint8_t len, uint8_t *data)
{
    if (config.spi)
    {
        spi_transaction_t *rxptr;
        trans.length = len * 8;
        trans.tx_buffer = data;
        if (receive_data)
        {
            trans.rxlength = len * 8;
        }
        else
        {
            trans.rxlength = 0;
        }
        ESP_ERROR_CHECK(spi_device_transmit((spi_device_handle_t)config.spi, &trans));
        if (receive_data)
        {
            ESP_ERROR_CHECK(spi_device_get_trans_result((spi_device_handle_t)config.spi, &rxptr, 100));
            memcpy(data, trans.rx_buffer, len);
        }
    }
}