// Dynamic memory pool for ESP32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // dynmem_start
#include "internal.h"

volatile static char dynmem_pool[20 * 1024]={0};

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    return (void*)dynmem_pool;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    return (void*)&dynmem_pool[sizeof(dynmem_pool)-10];
}
