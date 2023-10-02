# Additional ESP32 build rules
#Additional tools
AR=$(CROSS_PREFIX)ar
#Setup toolchain
prefix-$(CONFIG_MACH_ESP32_RAW)=$(esp_env) && xtensa-esp32-elf-
CROSS_PREFIX=$(prefix-y)

esp_target-$(CONFIG_MACH_ESP32_RAW)="esp32"
ESP_TARGET=$(esp_target-y)

## commnon defines
IDF_INCLUDES := -I $(OUT)ESP_BUILD/build/config
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/newlib/platform_include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/$(ESP_TARGET)
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/soc/$(ESP_TARGET)/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/$(ESP_TARGET)/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/hal/plateform_port/include/hal
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/include/$(ESP_TARGET)
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/esp_rom/$(ESP_TARGET)
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/xtensa/include
IDF_INCLUDES += -I $(CONFIG_PATH_TO_ESP32_IDF)/components/xtensa/$(ESP_TARGET)/include

CFLAGS := $(IDF_INCLUDES) $(CFLAGS)
CFLAGS += -mlongcalls 
# CFLAGS += -ffunction-sections -fdata-sectionss
CFLAGS += -DESP_PLATFORM -DIDF_VER="v5.1" -DSOC_MMU_PAGE_SIZE=CONFIG_MMU_PAGE_SIZE
CFLAGS += -D_GNU_SOURCE -D_POSIX_READER_WRITER_LOCKS

## common sources
dirs-y += src/esp src/generic

src-y += esp/gpio.c esp/timer.c esp/serial.c
src-y += esp/prebuilt.c
src-y += generic/crc16_ccitt.c generic/alloc.c
src-y += generic/timer_irq.c generic/serial_irq.c
## board spec sources
## chip spec sources
dirs-$(CONFIG_MACH_ESP32_RAW) += lib/


# build contents
target-y := $(OUT)ESP_PREBUILD.o
target-y += $(OUT)ESP_BUILD/main/libprebuilt.a
target-y += $(OUT)ESP_BUILD.o

esp_env=export IDF_PATH=$(CONFIG_PATH_TO_ESP32_IDF) && . $(CONFIG_PATH_TO_ESP32_IDF)/export.sh >> null
# build methods
$(OUT)ESP_PREBUILD.o:
	@echo " Seting Up ESP-IDF Environment"
 
	@echo " Copying ESP-IDF Project"
# Main Entry
	$(Q)cp -rf $(CONFIG_PATH_TO_ESP32_IDF)/examples/build_system/cmake/import_prebuilt $(OUT)ESP_BUILD
	$(Q)cp -f $(OUT)ESP_BUILD/prebuilt/components/prebuilt/prebuilt.h $(OUT)ESP_BUILD/main/
	$(Q)$(esp_env) && cd $(OUT)ESP_BUILD && idf.py set-target $(ESP_TARGET)
	0>>$(OUT)ESP_PREBUILD.o

$(OUT)ESP_BUILD/main/libprebuilt.a:$(OBJS_klipper.elf)
	@echo "	Generating $@"
	$(Q)$(AR) rcs $@ $(OBJS_klipper.elf) 

$(OUT)ESP_BUILD.o:$(OUT)ESP_BUILD/main/libprebuilt.a
	@echo " Building ESP_IDF Project"
	$(Q)$(esp_env) && cd $(OUT)ESP_BUILD && idf.py build
	0>>$(OUT)ESP_BUILD.o
esp_config:$(OUT)ESP_PREBUILD.o
	$(Q)$(esp_env) && cd $(OUT)ESP_BUILD && idf.py menuconfig
	
flash:
	@echo " Flashing into Target"
	$(Q)$(esp_env) && cd $(OUT)ESP_BUILD && idf.py -p $(CONFIG_ESP32_COM_PORT) flash
