//
// Created by Zixiong Liu on 7/3/17.
//

#ifndef MYOS_SYSCALL_H
#define MYOS_SYSCALL_H

#include <cstdint>

void syscall_init();

struct syscall_frame {
    uint64_t r9;
    uint64_t r8;
    uint64_t r10;
    uint64_t rdx;
    uint64_t rsi;
    uint64_t rdi;
    uint64_t rax;
};

#endif //MYOS_SYSCALL_H
