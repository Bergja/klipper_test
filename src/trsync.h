#ifndef __TRSYNC_H
#define __TRSYNC_H

#include <stdint.h> // uint16_t

struct trsync_signal;
typedef void (*trsync_callback_t)(volatile struct trsync_signal *tss, uint8_t reason);

struct trsync_signal {
    volatile struct trsync_signal *next;
    trsync_callback_t func;
};

volatile struct trsync *trsync_oid_lookup(uint8_t oid);
void trsync_do_trigger(volatile struct trsync *ts, uint8_t reason);
void trsync_add_signal(volatile struct trsync *ts, volatile struct trsync_signal *tss
                       , trsync_callback_t func);

#endif // trsync.h
