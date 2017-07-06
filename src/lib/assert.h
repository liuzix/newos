//
// Created by Zixiong Liu on 12/16/16.
//

#ifndef MYOS_ASSERT_H
#define MYOS_ASSERT_H

#include "printf.h"

inline void panic() {
  kprintf("PANIC");
  while (true) {
    //asm("hlt");
  }
}

#define assert_true(c) __assert_true(c, __FILE__, __LINE__)

inline void __assert_true(bool cond, const char *file, int line) {
  if (!cond) {
    kprintf("Assertion failed at %s line %d\n", file, line);
    panic();
  }
}


#endif //MYOS_ASSERT_H
