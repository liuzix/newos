//
// Created by Zixiong Liu on 6/18/17.
//

#ifndef MYOS_X86_H
#define MYOS_X86_H
#include <stdint.h>
namespace x86 {
    inline void outb(uint16_t port, uint8_t val) {
        __asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
        return;
    }

    inline void outl(uint16_t port, uint32_t val) {
        __asm volatile ("outl %0, %1" : : "eax"(val), "Nd"(port));
        return;
    }

    inline uint32_t inl(uint16_t port) {
        uint32_t val;
        __asm volatile ("in %1, %%eax; movl %%eax, %0" : "=g"(val) : "Nd"(port));
        return val;
    }

    inline uint8_t inb(uint16_t port) {
        register uint8_t val asm("al");
        __asm volatile("in %1, %0" : "=r"(val) : "Nd"(port));
        return val;
    }

    inline __attribute__ ((always_inline)) void set_rsp (uint64_t addr)  {
        asm ("mov %0, %%rsp" :: "r"(addr));
    }

    inline uint64_t rdmsr(uint32_t msr_id) {
        uint64_t value_hi, value_lo;
        asm volatile ( "rdmsr" : "=a" (value_lo), "=d" (value_hi) : "c" (msr_id) );
        return value_hi << 32 | value_lo;
    }

    inline uint64_t wrmsr(uint32_t msr_id, uint64_t msr_value) {
        uint64_t value_hi = msr_value >> 32;
        uint64_t value_lo = msr_value;
        asm volatile ( "wrmsr" ::  "c" (msr_id), "a" (value_lo), "d" (value_hi)  );
        return value_hi << 32 | value_lo;
    }

    inline void cpuid(unsigned int *eax, unsigned int *ebx,
                             unsigned int *ecx, unsigned int *edx)
    {
        /* ecx is often an input as well as an output. */
        asm volatile("cpuid"
        : "=a" (*eax),
        "=b" (*ebx),
        "=c" (*ecx),
        "=d" (*edx)
        : "0" (*eax), "2" (*ecx));
    }

    static inline uint64_t
    readeflags(void)
    {
        uint64_t eflags;
        asm volatile("pushf; pop %0" : "=r" (eflags));
        return eflags;
    }

    inline void sti () {
        asm volatile ("sti");
    }

    inline void cli () {
        asm volatile ("cli");
    }

    bool get_interrupt_status();
}

#endif //MYOS_X86_H
