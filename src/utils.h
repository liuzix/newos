//
// Created by Zixiong Liu on 12/16/16.
//

#ifndef MYOS_UTILS_H
#define MYOS_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <sched.h>
#include "lib/printf.h"

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

void inline _memset(void *s, int c, size_t n) {
	memset(s, c, n);
}

inline void sleep() {
	for (int i = 0; i < 20000; i++)
		kprintf("waiting...\n");
}


#endif //MYOS_UTILS_H
