//
// Created by Zixiong Liu on 6/18/17.
//

#include "apic.h"
#include "stdint.h"
#include "../x86.h"
#include "../lib/printf.h"
#include "../mmu.h"
#include "../idt.h"
#include "../smp.h"
#include "../thread.h"

#define IA32_APIC_BASE 0x1b
#define MMIO_APIC_BASE 0xfee00000
#define MSR_APIC_BASE 0x800
#define APIC_DIV_CONF 0x3E
#define APIC_LVT_TIMER 0x32
#define APIC_INIT_COUNT 0x38
#define APIC_CUR_COUNT 0x39
#define APIC_LVT_SPUR 0xF


static bool x2_apic_mode = false;
void write_apic_register_32(uint32_t offset, uint32_t value);

void init_apic () {
    unsigned int eax = 1, ebx = 0, ecx = 0, edx = 0;
    x86::cpuid(&eax, &ebx, &ecx, &edx);
    if ((ecx >> 21) & 1) {
        kprintf("x2apic detected\n");
        x2_apic_mode = true;
        uint64_t base_msr = x86::rdmsr(IA32_APIC_BASE);
        base_msr |= 1 << 10;
        x86::wrmsr(IA32_APIC_BASE, base_msr);
    } else {
        kprintf("no x2apic\n");
        x2_apic_mode = false;
        auto entry = memory::PML4E::get_current_PML4E()->try_map(MMIO_APIC_BASE, MMIO_APIC_BASE);
        entry->cache_disabled = true;
        memory::flush_tlb();
        // TODO: no cache
    }

    x86::outb(0xa1, 0xff);
    x86::outb(0x21, 0xff);

    write_apic_register_32(APIC_LVT_SPUR, 1 << 8 | 20);

}

void write_apic_register_64(uint32_t offset, uint64_t value) {
    if (x2_apic_mode) {
        x86::wrmsr(MSR_APIC_BASE + offset, value);
    } else {
        uint64_t* reg = (uint64_t*) (MMIO_APIC_BASE + (offset << 4));
        *reg = value;
    }
}

void write_apic_register_32(uint32_t offset, uint32_t value) {
    if (x2_apic_mode) {
        x86::wrmsr(MSR_APIC_BASE + offset, value);
    } else {
        uint32_t* reg = (uint32_t*) (MMIO_APIC_BASE + (offset << 4));
        *reg = value;
    }
}

uint64_t read_apic_register (uint32_t offset) {
    if (x2_apic_mode) {
        return x86::rdmsr(MSR_APIC_BASE + offset);
    } else {
        uint64_t* reg = (uint64_t*) (MMIO_APIC_BASE + (offset << 4));
        return *reg;
    }
}

uint64_t get_local_apic_id () {
    if (x2_apic_mode) {
        return read_apic_register(0x2);
    } else {
        return (uint32_t)read_apic_register(0x2)  >> 24;
    }
}

void apic_eoi() {
    write_apic_register_32(0xB, 0);
}

void timed_delay (uint32_t interval) {
    uint8_t port_61 = x86::inb(0x61);
    port_61 &= 0xD;
    port_61 |= 0x1;
    x86::outb(0x61, port_61);
    x86::outb(0x43, 0xB0);

    uint32_t latch_value = (1193182 * interval) / 1000000;
    x86::outb(0x42, (uint8_t)latch_value);
    x86::outb(0x42, (uint8_t)(latch_value >> 8));

    while ((x86::inb(0x61) & 0x20) == 0) {
        //kprintf("waiting\n");
    }

    port_61 = x86::inb(0x61);
    port_61 &= 0xC;
    x86::outb(0x61, port_61);
}



void timer_handler() {
    //kprintf("Timer interrupt from cpu %d\n", get_cpu_id());
    apic_eoi();
    proc::thread_tick();
}
static SpinLock lock;
void enable_timer () {
    static int64_t load_count = 0;

    lock.lock();
    write_apic_register_32(APIC_DIV_CONF, 3); //divisor = 3
    const uint64_t calib_init = 10000000;
    write_apic_register_32(APIC_LVT_TIMER, 1 << 17 | TIMER_INTERRUPT_VECTOR);
    if (load_count == 0) {

        write_apic_register_32(APIC_INIT_COUNT, calib_init);
        timed_delay(5000); // delay 5ms
        uint64_t cur_count = read_apic_register(APIC_CUR_COUNT) & 0xffffffff;
        int64_t diff = calib_init - cur_count;
        int64_t div = diff / 5;
        //kprintf("diff = %d\n", diff);
        load_count = (div) * TIMER_INTERVAL;
        kprintf("load_count = %d\n", load_count);
    }
    write_apic_register_32(APIC_INIT_COUNT, (uint32_t )load_count);
    kprintf("Cpu %d enabled timer, load_count = %u\n", get_cpu_id(), load_count);
    lock.unlock();
    x86::sti();
}

void global_init_apic() {
    register_handler(TIMER_INTERRUPT_VECTOR, timer_handler);
}