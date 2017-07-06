#include <stdio.h>

int main () {
    const char* str = "Hello World";
    for(int i = 0; i < 10000000; i++);
    asm volatile ("mov $100, %%rax; syscall" ::: "rax");
    asm volatile ("mov $60, %%rax; syscall" ::: "rax");
    return 0;
}

