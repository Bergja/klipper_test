// Dynamic memory pool for ESP32
//
// Copyright (C) 2023  Zhe Zhuang <chelieo@aol.com>
//
// This file may be distributed under the terms of the GNU GPLv3 license.

#include "board/misc.h" // dynmem_start
#include "internal.h"

static char dynmem_pool[20 * 1024];

// Return the start of memory available for dynamic allocations
void *
dynmem_start(void)
{
    if(dynmem_pool>(&dynmem_pool[sizeof(dynmem_pool)])){
        DEBUGI("KlipperMem","Reverse");
        return &dynmem_pool[sizeof(dynmem_pool)];
    }
    return dynmem_pool;
}

// Return the end of memory available for dynamic allocations
void *
dynmem_end(void)
{
    if(dynmem_pool>(&dynmem_pool[sizeof(dynmem_pool)])){
        return dynmem_pool;
    }
    return &dynmem_pool[sizeof(dynmem_pool)];
}
