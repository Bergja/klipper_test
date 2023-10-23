// Interrupt Handling functions on esp32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.
#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "internal.h"

static portMUX_TYPE klipper_spinlock = portMUX_INITIALIZER_UNLOCKED;
extern uint8_t esp_irq_stat;
static inline void __irq_disable(void)
{
    taskENTER_CRITICAL(&klipper_spinlock);
}

static inline void __irq_enable(void)
{
    taskEXIT_CRITICAL(&klipper_spinlock);
}

static inline void irq_disable(void)
{

    // taskENTER_CRITICAL(&klipper_spinlock);
    if (esp_irq_stat)
    {
        // DEBUGI("irq","Disabled!");
        // taskENTER_CRITICAL(&klipper_spinlock);
        esp_irq_stat = 0;
    }
}

static inline void irq_enable(void)
{
    // taskEXIT_CRITICAL(&klipper_spinlock);
    if (!esp_irq_stat)
    {
        // taskEXIT_CRITICAL(&klipper_spinlock);
        esp_irq_stat = 1;
        // DEBUGI("irq","Enabled!");
    }
}

typedef uint8_t irqstatus_t;

static inline irqstatus_t irq_save(void)
{
    return 0;
}

static inline void irq_restore(irqstatus_t flag)
{
    irq_enable();
}

static inline void irq_wait(void)
{
}

static inline void irq_poll(void)
{
}
