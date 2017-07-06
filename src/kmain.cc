//
// Created by Zixiong Liu on 12/16/16.
//
#include <stdint.h>
#include "vga.h"
#include "lib/printf.h"
#include "devices/serial.h"
#include "utils.h"
#include "string.h"
#include "multiboot.h"
#include "frame_alloc.h"
#include "mmu.h"
#include "smp.h"
#include "x86.h"
#include "devices/apic.h"
#include "idt.h"
#include "malloc.h"
#include "gdt.h"
#include "thread.h"
#include "lib/linkedlist.h"
#include "fs/ramdisk.h"
#include "elf.h"
#include "userproc.h"
#include "syscall.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"

void test_thread() {
    kprintf("This is test thread by CPU %d\n", get_cpu_id());
    kprintf("Let's put it to sleep\n");
    proc::scheduler->current.get()->sleep(5000);
    kprintf("Woke up!\n");
    LinkedList<testing_dummy> mylist;
    for (int i = 0; i < 1000; i++)
        mylist.push_front(new testing_dummy(i));

    for (auto a : mylist) {
    //    kprintf("%d\n", a->value);
    }
}

void initial_thread() {
    global_filesystem_init();
    proc::Userproc::new_userprog("/ramdisk/userprog/test");
}

extern "C" int kmain(intptr_t boot_info) {
	void* boot_info_temp = (void*)boot_info; // save a copy in case rdi destroyed
	init_serial();
	kprintf("system loading...\n");
	kprintf("kprintf: boot_info = %u \n", boot_info_temp);
	multiboot::parse_tags(boot_info);
    kprintf("PML4E addr = 0x%lx\n", memory::PML4E::get_current_PML4E());
    x86::set_rsp(memory::get_and_map_multiple(5) + 5 * 4096 - 16);
    kprintf("Reset stack. \n");

    init_gdt();
    init_idt();
    kprintf("Reset GDT\n");
    init_apic();
    register_cpu();
    global_init_apic();

    for (int j = 0; j < 1000; j++)
        timed_delay(10000);

    int i = 0;
    memory::malloc::malloc_init();
    mpboot_init();
    for (int j = 0; j < 10; j++)
        timed_delay(10000);
    asm ("int $3");
    proc::scheduler_init();
    create_and_set_tss();
    syscall_init();
    //proc::Thread::new_thread(test_thread, "test thread1");
    proc::Thread::new_thread(initial_thread, "initial thread");

    proc::enter_schedule_loop();

    while (true) {
        i++;
    }
  	asm("cli");
	while (true) {
		asm("hlt");
	}
	// When the first interrupt comes, idle thread will be run
	// NOT REACHED
}



extern "C" uint64_t create_stack() {
    return memory::get_and_map_multiple(5) + 5 * 4096 - 16;
}

#pragma clang diagnostic pop
