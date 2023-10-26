# Additional ESP32 build rules
#Additional tools
SHELL=/bin/bash
AR=$(CROSS_PREFIX)ar
#Setup toolchain
prefix-$(CONFIG_MACH_ESP32_RAW)=$(esp_env) && xtensa-esp32-elf-
CROSS_PREFIX=$(prefix-y)

esp_target-$(CONFIG_MACH_ESP32_RAW)="esp32"
ESP_TARGET=$(esp_target-y)

## commnon defines
IDF_INCLUDES-y := -I $(OUT)KlipperESPProject/build/config


IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/xtensa/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/xtensa/$(ESP_TARGET)/include

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/$(ESP_TARGET)
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/$(ESP_TARGET)/include

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/$(ESP_TARGET)/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/include
# IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/plateform_port/include/hal

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/include/$(ESP_TARGET)
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/$(ESP_TARGET)

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/freertos/FreeRTOS-Kernel/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/freertos/FreeRTOS-Kernel/portable/xtensa/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/freertos/esp_additions/include/freertos 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/freertos/esp_additions/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/freertos/esp_additions/arch/xtensa/include 

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hw_support/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hw_support/include/soc 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hw_support/include/soc/$(ESP_TARGET)
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hw_support/port/$(ESP_TARGET)/
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hw_support/port/$(ESP_TARGET)/private_include

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/efuse/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/efuse/$(ESP_TARGET)/include 

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/include/apps
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/include/apps/sntp
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/lwip/src/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/port/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/port/freertos/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/port/esp32xx/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/lwip/port/esp32xx/include/arch

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/heap/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/log/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_timer/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_common/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_system/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_system/port/soc 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_system/port/include/private 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_ringbuf/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_pm/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_app_format/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/bootloader_support/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/bootloader_support/bootloader_flash/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_partition/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/app_update/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_mm/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/spi_flash/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/pthread/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_timer/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/app_trace/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_event/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/nvs_flash/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_phy/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_phy/$(ESP_TARGET)/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/vfs/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_netif/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/wpa_supplicant/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/wpa_supplicant/port/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/wpa_supplicant/esp_supplicant/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_coex/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_wifi/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_wifi/wifi_apps/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/unity/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/unity/unity/src
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/cmock/CMock/src
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/console
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/http_parser
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp-tls
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp-tls/esp-tls-crypto
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_adc/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_adc/interface
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_adc/$(ESP_TARGET)/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_adc/deprecated/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_eth/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_gdbstub/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_hid/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/tcp_transport/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_http_client/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_http_server/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_https_ota/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_psram/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_lcd/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_lcd/interface
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/protobuf-c/protobuf-c
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/protocomm/include/common
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/protocomm/include/security
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/protocomm/include/transports
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_local_ctrl/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/espcoredump/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/espcoredump/include/port/xtensa
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/wear_levelling/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/sdmmc/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/fatfs/diskio
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/fatfs/vfs
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/fatfs/src
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/idf_test/include

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/newlib/platform_include

IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/deprecated 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/analog_comparator/include 
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/dac/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/gpio/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/gptimer/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/i2c/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/i2s/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/ledc/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/mcpwm/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/parlio/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/pcnt/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/rmt/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/sdio_slave/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/sdmmc/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/sigma_delta/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/spi/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/temperature_sensor/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/touch_sensor/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/twai/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/uart/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/usb_serial_jtag/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/driver/touch_sensor/$(ESP_TARGET)/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/idf_test/include/$(ESP_TARGET)
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/ieee802154/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/json/cJSON
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/mqtt/esp-mqtt/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/perfmon/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/spiffs/include
IDF_INCLUDES-y += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/wifi_provisioning/include

CFLAGS := $(IDF_INCLUDES-y) $(CFLAGS)
CFLAGS += -mlongcalls 
# CFLAGS += -ffunction-sections -fdata-sectionss
CFLAGS += -DESP_PLATFORM -DIDF_VER="v5.1.1" -DSOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE
CFLAGS += -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS

## common sources
dirs-y += src/esp src/generic

# peripherals code
src-y += esp/timer.c esp/serial.c # System Base
src-y += esp/gpio.c esp/spi.c esp/pwm.c esp/adc.c 
src-$(CONFIG_HAVE_GPIO_I2C) += esp/i2c.c# IO Functions
src-y += generic/timer_irq.c
# Main func
src-y += esp/prebuilt.c
# general functions
src-y += generic/crc16_ccitt.c esp/alloc.c
## board spec sources
## chip spec sources
dirs-$(CONFIG_MACH_ESP32_RAW) += lib/


# build contents
target-y := $(OUT)ESP_PREBUILD.o
target-y += $(OUT)KlipperESPProject/main/libprebuilt.a
target-y += $(OUT)ESP_BUILD.o

esp_env=export IDF_PATH=$(CONFIG_PATH_TO_ESP32_IDF) && . $(CONFIG_PATH_TO_ESP32_IDF)/export.sh >> /dev/null
# build methods
$(OUT)ESP_PREBUILD.o:
	@echo " Seting Up ESP-IDF Environment"
 
	@echo " Copying ESP-IDF Project"
# Main Entry
	$(Q)cp -rf src/esp/KlipperESPProject $(OUT)
	$(Q)cp -f src/esp/prebuilt.h $(OUT)KlipperESPProject/main/
	$(Q)cp -f $(OUT)KlipperESPProject/sdkconfig.defaults.$(ESP_TARGET) $(OUT)KlipperESPProject/sdkconfig
	$(Q)$(esp_env) && cd $(OUT)KlipperESPProject/ && idf.py -DIDF_TARGET=$(ESP_TARGET) reconfigure
	0>>$(OUT)ESP_PREBUILD.o

$(OUT)KlipperESPProject/main/libprebuilt.a:$(OBJS_klipper.elf)
	@echo "	Generating $@"
	$(Q)$(AR) rcs $@ $(OBJS_klipper.elf) 

$(OUT)ESP_BUILD.o:$(OUT)KlipperESPProject/main/libprebuilt.a
	@echo " Building ESP_IDF Project"
	$(Q)$(esp_env) && cd $(OUT)KlipperESPProject/ && idf.py build
	0>>$(OUT)ESP_BUILD.o

esp_config:$(OUT)ESP_PREBUILD.o
	$(Q)$(esp_env) && cd $(OUT)KlipperESPProject/ && idf.py menuconfig
	$(Q)cp -f $(OUT)KlipperESPProject/sdkconfig src/esp/KlipperESPProject/sdkconfig.defaults.$(ESP_TARGET)
	$(Q)rm -f $(OUT)ESP_PREBUILD.o

quick:$(OUT)KlipperESPProject/main/libprebuilt.a
	@echo "Quick Build Done"
	
flash:
	make
	@echo " Flashing into Target"
	$(Q)$(esp_env) && cd $(OUT)KlipperESPProject/ && idf.py -p $(CONFIG_ESP32_COM_PORT) flash

develop:
	@echo " Switching to Develop Setup"
	$(Q)cp -f src/esp/KlipperESPProject/sdkconfig.develop.defaults.esp32 src/esp/KlipperESPProject/sdkconfig.defaults.esp32
	$(Q)cp -f .config.dev .config
	make clean
	make


set-develop:
	@echo " Saving config to Develop Setup"
	$(Q)cp -f src/esp/KlipperESPProject/sdkconfig.defaults.esp32 src/esp/KlipperESPProject/sdkconfig.develop.defaults.esp32 
	$(Q)cp -f .config .config.dev


release:
	@echo " Switching to Release Setup"
	$(Q)cp -f src/esp/KlipperESPProject/sdkconfig.release.defaults.esp32 src/esp/KlipperESPProject/sdkconfig.defaults.esp32
	$(Q)cp -f .config.release .config
	make clean
	make

set-release:
	@echo " Saving config to Release Setup"
	$(Q)cp -f src/esp/KlipperESPProject/sdkconfig.defaults.esp32 src/esp/KlipperESPProject/sdkconfig.release.defaults.esp32
	$(Q)cp -f .config .config.release 