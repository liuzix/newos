//
// Created by Zixiong Liu on 6/7/17.
//

#include "frame_alloc.h"
#include "lib/assert.h"
#include "utils.h"


namespace memory {

    size_t memory::PageAllocator::allocate() {
        guard.lock();
        size_t start_clock = clock;
        while (this->freemap.get(clock)) {

            clock += 1;
            if (clock >= freemap.capacity) {
                clock = 0;
            }
            if (clock == start_clock) {
                kprintf("out of physical memory!\n");
                panic();
            }
        }
        freemap.set(clock, true);
        guard.unlock();
        return clock * 4096 + baseAddr;
    }

    void memory::PageAllocator::deallocate(size_t addr) {
        guard.lock();
        size_t index = (addr - baseAddr) / 4096;
        assert_true(index < freemap.capacity);
        if (!freemap.get(index)) {
            kprintf("Trying to free page 0x%lx, which is already free.\n", addr);
            panic();
        } else {
            freemap.set(index, false);
        }
        guard.unlock();
    }

    static PageAllocator *physical_page_allocator = nullptr;

    PageAllocator *get_physical_allocator() {
        if (physical_page_allocator == nullptr) {
            // lazily initialize
            size_t base_addr = (size_t)multiboot::get_first_free_page();
            size_t memory_len = (size_t)multiboot::memory_base + (size_t)multiboot::memory_length - base_addr;
            size_t allocator_overhead = memory_len / (4096 * 8) + sizeof(PageAllocator);
            size_t first_real_page = (base_addr + allocator_overhead + 4095) & (-1 << 12);
            size_t real_mem_len = (memory_len - allocator_overhead) / 4096;

            kprintf ("base_Addr = 0x%lx, memory_len = 0x%lx, first_real_page = 0x%lx\n",
                base_addr, memory_len, first_real_page);

            physical_page_allocator = new ((void*)base_addr) PageAllocator(real_mem_len, first_real_page);
        }
        return physical_page_allocator;
    }

    void test_allocator () {
        static size_t pages[1000];
        for (int i = 0; i < 1000; i++) {
            pages[i] = get_physical_allocator()->allocate();
            kprintf("Got page 0x%lx\n", pages[i]);
        }
        for (int i = 0; i < 1000; i++) {
            kprintf("Freeing page 0x%lx\n", pages[i]);
            get_physical_allocator()->deallocate(pages[i]);
        }
    }



}