//
// Created by Zixiong Liu on 12/18/16.
//

// miscellaneous runtime support
#include <stdint.h>
#include <stddef.h>
#include <exception>
#include <stdexcept>
#include "malloc.h"
#include "lib/assert.h"

extern "C" void __assert_func(const char* file, int line, const char* func, const char* what) {
  //if (!cond) {
    kprintf("Assertion failed at %s line %d in %s: %s\n", file, line, func, what);
    panic();
  //}

}
extern "C"  void __assert_fail(const char * assertion, const char * file, unsigned int line, const char * function) {
    kprintf("Assertion failed at %s line %d in %s: %s\n", file, line, function, assertion);
}

/*
extern "C"  void* memmove(void* dstptr, const void* srcptr, size_t size) {
    unsigned char* dst = (unsigned char*) dstptr;
    const unsigned char* src = (const unsigned char*) srcptr;
    if (dst < src) {
        for (size_t i = 0; i < size; i++)
            dst[i] = src[i];
    } else {
        for (size_t i = size; i != 0; i--)
            dst[i-1] = src[i-1];
    }
    return dstptr;
}

extern "C" void *memset(void *s, int c, size_t n) {
  unsigned char* p;
  p = (uint8_t *)s;
  while(n--)
    *p++ = (unsigned char)c;
  return s;
}

extern "C" void memcpy(void* dest, void* src, int size)
{
    uint8_t *pdest = (uint8_t*) dest;
    uint8_t *psrc = (uint8_t*) src;

    int loops = (size / sizeof(uint32_t));
    for(int index = 0; index < loops; ++index)
    {
        *((uint32_t*)pdest) = *((uint32_t*)psrc);
        pdest += sizeof(uint32_t);
        psrc += sizeof(uint32_t);
    }

    loops = (size % sizeof(uint32_t));
    for (int index = 0; index < loops; ++index)
    {
        *pdest = *psrc;
        ++pdest;
        ++psrc;
    }
}
*/

namespace boost{
    void throw_exception(std::exception const &e){}
}

void *operator new(size_t size)
{
    return memory::malloc::malloc(size);
}

void *operator new[](size_t size)
{
    return memory::malloc::malloc(size);
}

void operator delete(void *p)
{
    memory::malloc::free(p);
}

void operator delete[](void *p)
{
    memory::malloc::free(p);
}

void operator delete(void* p, unsigned long) {
    memory::malloc::free(p);
}

namespace __cxxabiv1
{
    /* guard variables */

    /* The ABI requires a 64-bit type.  */
    __extension__ typedef int __guard __attribute__((mode(__DI__)));

    extern "C" int __cxa_guard_acquire (__guard *);
    extern "C" void __cxa_guard_release (__guard *);
    extern "C" void __cxa_guard_abort (__guard *);

    extern "C" int __cxa_guard_acquire (__guard *g)
    {
        return !*(char *)(g);
    }

    extern "C" void __cxa_guard_release (__guard *g)
    {
        *(char *)g = 1;
    }

    extern "C" void __cxa_guard_abort (__guard *)
    {

    }

    extern "C" void __cxa_pure_virtual() { assert_true(false);}
}


namespace std {

}


//extern "C" void __dso_handle() {
//  return;
//}