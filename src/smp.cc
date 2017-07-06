//
// Created by Zixiong Liu on 6/17/17.
//
#include <string.h>
#include "x86.h"
#include "lib/printf.h"
#include "devices/apic.h"
#include "lib/assert.h"
#include "idt.h"
#include "gdt.h"
#include "thread.h"
#include "syscall.h"

#define MAX_CPU_NUM 48

#define MP_COPY_TO 0x1000
#define APIC_ICR_OFFSET 0x30
extern const char mp_start[], mp_end[];

void mpboot_init () {
    memcpy((void*)MP_COPY_TO, mp_start, mp_end - mp_start);
    uint64_t  vector_no = (MP_COPY_TO >> 12) & 0xFF;
    write_apic_register_64(APIC_ICR_OFFSET, 0xC4500);
    for (int i = 0; i < 10000; i++);
    write_apic_register_64(APIC_ICR_OFFSET, 0xC4600 | vector_no);
    for (int i = 0; i < 10000; i++);
    write_apic_register_64(APIC_ICR_OFFSET, 0xC4600 | vector_no);
}


static int cpu_id[MAX_CPU_NUM];
static int max_id = 0;

void register_cpu() {
    int myid = __sync_fetch_and_add(&max_id, 1);
    uint64_t apic_id = get_local_apic_id();
    kprintf("CPU APIC ID = 0x%lx\n", apic_id);
    assert_true(apic_id < MAX_CPU_NUM);
    cpu_id[apic_id] = myid;
}

int get_cpu_id() {
    uint64_t apic_id = get_local_apic_id();
    return cpu_id[apic_id];
}

int get_num_cpu() {
    return max_id + 1;
}


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
extern "C" int mp_main() {
    init_apic();
    register_cpu();
    init_idt();
    init_gdt();
    //enable_timer ();
    for (int j = 0; j < 10000; j++) {
        asm volatile ("pause;");
    }
    create_and_set_tss();
    syscall_init();
    proc::enter_schedule_loop();
    while (true) {
        asm("hlt");
    }
}
#pragma clang diagnostic pop