// GPIO functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#include "gpio.h"
#include "command.h"
//!Simulation
#define ESP32_IO_MAX_NUM 39

#ifdef ESP32_IO_EXPANSION//Pin Expansion With 74HCxx
    #define ESP32_EXP_IO_DATA_PIN 2//!Configured in Kconfig
    #define ESP32_EXP_IO_SRCLK_PIN 3
    #define ESP32_EXP_IO_RCLK_PIN 4
    #define ESP32_EXP_IO_COUNT 24
    #ifdef ESP32_IO_DUAL_EXPANSION
        #define ESP32_EXP2_IO_RCLK_PIN 5//!Configured in Kconfig
        #define ESP32_EXP2_IO_COUNT 32
    #endif
#endif

uint32_t invalid_pins[4]={
    0x0,0x0,0x0,0x0
};


DECL_ENUMERATION_RANGE("pin", "PA0",0,39);
#ifdef ESP32_IO_EXPANSION
DECL_ENUMERATION_RANGE("pin","PB0",ESP32_IO_MAX_NUM,ESP32_EXP_IO_COUNT);
#ifdef ESP32_IO_DUAL_EXPANSION
DECL_ENUMERATION_RANGE("pin","PC0",ESP32_IO_MAX_NUM+ESP32_EXP_IO_COUNT,ESP32_EXP2_IO_COUNT);
#endif
#endif



struct gpio_out gpio_out_setup(uint32_t pin, uint32_t val){
    return (struct gpio_out){.bit=pin};
}
void gpio_out_reset(struct gpio_out g, uint32_t val){
    
}
void gpio_out_toggle_noirq(struct gpio_out g){

}
void gpio_out_toggle(struct gpio_out g){

}
void gpio_out_write(struct gpio_out g, uint32_t val) {
}
struct gpio_in gpio_in_setup(uint32_t pin, int32_t pull_up){
    return (struct gpio_in){.bit=pin};
}
void gpio_in_reset(struct gpio_in g, int32_t pull_up){

}
uint8_t gpio_in_read(struct gpio_in g){
    return 0;
}
struct gpio_pwm gpio_pwm_setup(uint8_t pin, uint32_t cycle_time, uint8_t val) {
    return (struct gpio_pwm){.reg=&pin};
}
void gpio_pwm_write(struct gpio_pwm g, uint32_t val) {
}
struct gpio_adc gpio_adc_setup(uint32_t pin) {
    return (struct gpio_adc){.chan=pin};
}
uint32_t gpio_adc_sample(struct gpio_adc g) {
    return 0;
}
uint16_t gpio_adc_read(struct gpio_adc g) {
    return 0;
}
void gpio_adc_cancel_sample(struct gpio_adc g) {
}

struct spi_config spi_setup(uint32_t bus, uint8_t mode, uint32_t rate)
{
    return (struct spi_config){ };
}
void spi_prepare(struct spi_config config)
{
}
void spi_transfer(struct spi_config config, uint8_t receive_data
             , uint8_t len, uint8_t *data){
}
