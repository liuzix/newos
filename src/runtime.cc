//
// Created by Zixiong Liu on 12/18/16.
//

// miscellaneous runtime support
#include <stdint.h>
#include <stddef.h>
#include "lib/assert.h"

extern "C" void *memset(void *s, int c, size_t n);




extern "C" void __assert_func(const char* file, int line, const char* func, const char* what) {
  //if (!cond) {
    kprintf("Assertion failed at %s line %d in %s: %s\n", file, line, func, what);
    panic();
  //}
}


extern "C" void *memset(void *s, int c, size_t n) {
  unsigned char* p;
  p = (uint8_t *)s;
  while(n--)
    *p++ = (unsigned char)c;
  return s;
}


extern "C" void __cxa_atexit() {
  return;
}

//extern "C" void __dso_handle() {
//  return;
//}