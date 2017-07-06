//
// Created by Zixiong Liu on 6/9/17.
//

#ifndef MYOS_MMU_H
#define MYOS_MMU_H
#include <stdint.h>
#include <string.h>

#define KERNEL_HEAP_BASE ((uint64_t)1 << 39)
#define USER_TEXT_BASE ((uint64_t)2 << 39)
#define USER_STACK_BASE ((uint64_t)2 << 40)
namespace memory {

    struct page_table_entry {
        bool present : 1;
        bool writable : 1;
        bool user : 1;
        bool write_through : 1;
        bool cache_disabled : 1;
        bool accessed : 1;
        bool dirty : 1;
        bool huge_page : 1;
        bool global : 1;
        int software : 3;
        uint64_t address : 40;
        int reserved : 12;
    } __attribute__((packed));

    struct PML4E {
        page_table_entry entries[64];

        page_table_entry * try_map(uint64_t vaddr, uint64_t paddr);
        page_table_entry * try_map_user(uint64_t vaddr);
        void direct_map_lower_2g();
        void load();
        ////
        static PML4E* get_current_PML4E();
    };
    uint64_t get_and_map_multiple(int num);
    inline void flush_tlb () {
        asm ("mov %%cr3, %%rax; mov %%rax, %%cr3" ::: "rax");
    }

    inline page_table_entry get_default_entry() {
        page_table_entry new_pe;
        memset(&new_pe, 0, sizeof(new_pe));
        new_pe.address = 0;
        new_pe.present = 1;
        new_pe.global = 0;
        new_pe.huge_page = 0;
        new_pe.user = 1;
        new_pe.writable = 1;
        return new_pe;
    }

    PML4E* create_user_vm ();
    void test_page_mapping ();
}

#endif //MYOS_MMU_H
