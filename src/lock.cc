//
// Created by Zixiong Liu on 6/7/17.
//

#include "lock.h"
#include "lib/assert.h"
#include "thread.h"

void synchronization::SpinLock::lock() {
    while (true) {
        bool current = __sync_lock_test_and_set(&this->lockvar, true);
        if (!current) break;
        else {

        }
    }
}

void synchronization::SpinLock::unlock() {
    assert_true(this->lockvar);
    __sync_lock_release(&this->lockvar);
}

bool synchronization::SpinLock::trylock() {
    return !__sync_lock_test_and_set(&this->lockvar, true);
}

synchronization::SpinLock::SpinLock() {
    this->lockvar = false;
}
