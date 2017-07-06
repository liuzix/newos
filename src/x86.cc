//
// Created by Zixiong Liu on 6/18/17.
//

#include "x86.h"


bool ::x86::get_interrupt_status() {
    uint64_t flags = readeflags();
    return ((flags & (1 << 9)) != 0);
}
