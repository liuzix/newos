//
// Created by Zixiong Liu on 6/20/17.
//

#ifndef MYOS_MALLOC_H
#define MYOS_MALLOC_H

#include "lock.h"

namespace memory::malloc {

    struct block {
        uint32_t magic;
        uint64_t size;
        bool free;
        block* next;

        block (uint64_t _size);
        block * split(uint64_t size_1);
        void merge ();
    };

    class arena {
        synchronization::SpinLock lock;
        block* head;
    public:
        void* allocate(uint64_t size);
        void free (void*);
        void init();

    };

    void* malloc (uint64_t size);
    void free (void* ptr);
    void malloc_init ();
    void test_malloc();
}
#endif //MYOS_MALLOC_H
