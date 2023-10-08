// Hardware SPI functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/gpio.h"
#include "autoconf.h"
#include "driver/spi_master.h"

#if CONFIG_ESP32_SPI_ENABLE && CONFIG_ESP_SPI_HSPI && CONFIG_ESP_SPI_VSPI
spi_host_device_t spi_bus[]={
    #if CONFIG_ESP_SPI_HSPI
    HSPI_HOST,
    #endif
    #if CONFIG_ESP_SPI_VSPI
    VSPI_HOST,
    #endif
};
#endif


struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    return (struct spi_config){};
}
inline void spi_prepare(struct spi_config config)
{
    //Do Nothing
}
void spi_transfer(struct spi_config config, uint8_t receive_data, uint8_t len, uint8_t *data)
{
}