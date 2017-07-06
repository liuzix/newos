//
// Created by Zixiong Liu on 12/16/16.
//

#ifndef MYOS_UTILS_H
#define MYOS_UTILS_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <new>
#include "lib/printf.h"


void inline _memset(void *s, int c, size_t n) {
	memset(s, c, n);
}

inline void sleep() {
	for (int i = 0; i < 20000; i++)
		kprintf("waiting...\n");
}

//inline void *operator new(size_t, void *p)     throw() { return p; }
//inline void *operator new[](size_t, void *p)   throw() { return p; }
//inline void  operator delete  (void *, void *) throw() { };
//inline void  operator delete[](void *, void *) throw() { };


#endif //MYOS_UTILS_H
