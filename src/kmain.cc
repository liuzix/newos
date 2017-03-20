//
// Created by Zixiong Liu on 12/16/16.
//
#include <stdint.h>
#include "vga.h"
#include "lib/printf.h"
#include "devices/serial.h"
#include "utils.h"
#include "string.h"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


extern "C" int kmain(int boot_info) {
	void* boot_info_temp = (void*)boot_info; // save a copy in case rdi destroyed
	init_serial();
	kprintf("system loading...\n");
	kprintf("kprintf: boot_info = %u \n", boot_info_temp);
	
  	asm("cli");
	while (true)
	{
		asm("hlt");
	}
	// When the first interrupt comes, idle thread will be run
	// NOT REACHED
	
}


#pragma clang diagnostic pop
