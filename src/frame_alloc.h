//
// Created by Zixiong Liu on 6/7/17.
//

#ifndef MYOS_PAGE_ALLOC_H
#define MYOS_PAGE_ALLOC_H

#include "lib/bitmap.h"
#include <stdint.h>
#include <stddef.h>
#include "lock.h"
#include "multiboot.h"

using namespace synchronization;

namespace memory {

    // this is a single page allocator
    class PageAllocator {
        size_t clock;
        SpinLock guard;
        size_t baseAddr;
        bitmap freemap;
    public:
        PageAllocator (size_t len, size_t _baseAddr) : freemap(len), clock(0), baseAddr(_baseAddr) {}
        size_t allocate ();
        void deallocate (size_t addr);
    };

    PageAllocator *get_physical_allocator();
    void test_allocator ();

}

#endif //MYOS_PAGE_ALLOC_H

