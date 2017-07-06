//
// Created by Zixiong Liu on 7/3/17.
//

#include "syscall.h"
#include "x86.h"
#include "lib/printf.h"
#include "lib/cpu_local.h"
#include "gdt.h"
#include "userproc.h"

#define IA32_EFER 0xC0000080
#define IA32_LSTAR 0xC0000082
#define IA32_STAR 0xC0000081
#define IA32_FMASK 0xC0000084

extern "C" void syscall_stub();
extern "C" void syscall_entry(syscall_frame* frame) {
    switch (frame->rax) {
        case 60:  // sys_exit
            proc::sys_exit(frame->rdi);
            assert_true(!"unreachable code!");
            break;
        default:
            kprintf("Unknown syscal number %d\n", frame->rax);
    }
}

static CPU_local<uint64_t> *user_rsp;
extern "C" uint64_t save_user_get_kernel_rsp(uint64_t u_stack) {
    user_rsp->set(u_stack);
    return get_ring0_rsp();
}

extern "C" uint64_t get_user_rsp() {
    return user_rsp->get();
}

void syscall_init() {
    user_rsp = new CPU_local<uint64_t>;
    auto efer = x86::rdmsr(IA32_EFER);
    efer |= 1;
    x86::wrmsr(IA32_EFER, efer);
    x86::wrmsr(IA32_LSTAR, (uint64_t)syscall_stub);
    x86::wrmsr(IA32_STAR, ((uint64_t)8 << 32) | ((uint64_t)16 << 48));
    x86::wrmsr(IA32_FMASK, 0);
}