#pragma once

#include <stdint.h>

typedef uint8_t spinlock_t;

enum {
    SPINLOCK_FREE = 0,
    SPINLOCK_LOCKED = 1
};

void acquire_lock(spinlock_t *lock);
void release_lock(spinlock_t *lock);
