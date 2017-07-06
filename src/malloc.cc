//
// Created by Zixiong Liu on 6/20/17.
//

#include <stdint.h>
#include "malloc.h"
#include "lib/assert.h"
#include "utils.h"
#include "mmu.h"
#include <boost/container/vector.hpp>
#include <boost/container/list.hpp>

#define BLOCK_MAGIC 0x233233


memory::malloc::block * memory::malloc::block::split(uint64_t size_1) {
    if (this->size < size_1 + sizeof(block)) return nullptr;
    //assert_true(this->size > size_1 + sizeof(block));
    assert_true(this->magic == BLOCK_MAGIC);
    assert_true(this->free);
    auto new_block = new ((char*)this + sizeof(block) + size_1) block(this->size - size_1 - sizeof(block));
    new_block->next = this->next;
    this->next = new_block;
    this->size = size_1;
    return new_block;
}

memory::malloc::block::block(uint64_t _size) {
    this->size = _size;
    this->free = true;
    this->magic = BLOCK_MAGIC;
    this->next = nullptr;
}

void memory::malloc::block::merge() {
    assert_true(this->magic == BLOCK_MAGIC);
    assert_true(this->free);

    if (this->next && ((uint64_t)this + sizeof(block) + this->size == (uint64_t)this->next)) {
        assert_true(this->next->free);
        this->next->magic = 0;
        this->size += sizeof(block) + this->next->size;
        this->next = this->next->next;
    }
}

void *memory::malloc::arena::allocate(uint64_t size) {
    auto iter = this->head;
    if (size == 0) return nullptr;
    uint64_t true_size = ((size - 1) / 8 + 1) * 8;
    this->lock.lock();
    block* prev = nullptr;
    while (iter && iter->size < true_size) {
        prev = iter;
        iter = iter->next;
    }
    if (iter != nullptr) {
        assert_true(iter->magic == BLOCK_MAGIC);
        iter->split(true_size);
        if (prev)
            prev->next = iter->next;
        if (this->head == iter)
            this->head = iter->next;
    } else {
        iter = new ((void*)get_and_map_multiple((int)((true_size + sizeof(block) - 1) / 4096 + 1)))
                block(((true_size + sizeof(block) - 1) / 4096 + 1) * 4096 - sizeof(block));
        //kprintf("Asking for %d pages\n", (true_size + sizeof(block) - 1) / 4096 + 1);
    }
    auto split_blk = iter->split(true_size);
    if (split_blk) {
        if (prev)
            prev->next = split_blk;
        else
            this->head = split_blk;
    }
    assert_true(iter->free);
    iter->free = false;
    iter->next = nullptr;
    assert_true(iter->magic == BLOCK_MAGIC);
    this->lock.unlock();
    //kprintf("allocating %d\n", true_size);
    return ((char*)iter + sizeof(block));
}

void memory::malloc::arena::free(void *ptr) {
    block* blk = (block*)((char*)ptr - sizeof(block));
    assert_true(blk->magic == BLOCK_MAGIC);
    assert_true(!blk->free);
    blk->free = true;
    //kprintf("freeing %d\n", blk->size);
    this->lock.lock();
    auto iter = this->head;
    block* prev = nullptr;
    if (!iter) {
        this->head = blk;
        blk->next = nullptr;
        goto done;
    }

    while (iter && (uint64_t)iter < (uint64_t)blk) {
        prev = iter;
        iter = iter->next;
    }
    if (iter) {
        blk->next = iter;
        blk->merge();
        if (prev) {
            prev->next = blk;
            prev->merge();
        } else {
            this->head = blk;
        }
    } else {
        assert_true(prev != nullptr);
        prev->next = blk;
        blk->next = nullptr;
    }
    done:
    this->lock.unlock();
}

void memory::malloc::arena::init() {
    this->head = nullptr;
}

namespace memory::malloc {
    static arena global_arena;
    void malloc_init () {
        global_arena.init();
        
    }

    void* malloc (uint64_t size) {
        return global_arena.allocate(size);
    }

    void free (void* ptr) {
        return global_arena.free(ptr);
    }


    void test_malloc() {
        using namespace boost::container;
        vector<int> test;
        for (int i = 0; i < 100000; i++) {
            test.push_back(i);
        }
        for (int i = 0; i < 100000; i++) {
            assert_true(test[i] == i);
        }

        list<int> mylist;
        for (int i = 0; i < 100000; i++) {
            mylist.push_back(i);
        }
        for (int i = 0; i < 100000; i++) {
            assert_true(mylist.front() == i);
            mylist.pop_front();
        }
        kprintf("malloc test passed\n");

    }
}
