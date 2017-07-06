//
// Created by Zixiong Liu on 6/9/17.
//

#include <string.h>
#include "mmu.h"
#include "frame_alloc.h"


namespace memory {
    PML4E* PML4E::get_current_PML4E() {
        uint64_t cr3_content = 0;
        asm ("movq %%cr3, %0;" : "=r" (cr3_content));
        cr3_content &= (-1 << 12);
        return (PML4E*)cr3_content;
    }

    page_table_entry * PML4E::try_map(uint64_t vaddr, uint64_t paddr) {
        uint64_t PLM4_index = (vaddr >> 39) & 0b111111111;
        uint64_t PDP_index = (vaddr >> 30) & 0b111111111;
        uint64_t PD_index = (vaddr >> 21) & 0b111111111;
        uint64_t PT_index = (vaddr >> 12) & 0b111111111;
        //kprintf("vaddr = 0x%lx, PLM4 = 0x%lx, PDP = 0x%lx, PD = 0x%lx, PT = 0x%lx\n",
        //    vaddr, PLM4_index, PDP_index, PD_index, PT_index);
        page_table_entry* PDP = nullptr;
        if (!entries[PLM4_index].present) {
            //kprintf("new PDP\n");
            PDP = (page_table_entry*)get_physical_allocator()->allocate();
            memset(PDP, 0, 4096);
            entries[PLM4_index] = get_default_entry();
            entries[PLM4_index].address = (uint64_t)PDP / 4096;
        } else {
            PDP = (page_table_entry*) (entries[PLM4_index].address * 4096);
        }
        //kprintf("PDP = 0x%lx\n", PDP);

        page_table_entry* PD = nullptr;
        if (!PDP[PDP_index].present) {
            //kprintf("new PD\n");
            PD = (page_table_entry*)get_physical_allocator()->allocate();
            memset(PD, 0, 4096);
            PDP[PDP_index] = get_default_entry();
            PDP[PDP_index].address = (uint64_t)PD / 4096;
        } else {
            PD = (page_table_entry*) (PDP[PDP_index].address * 4096);
        }
        //kprintf("PD = 0x%lx\n", PD);

        page_table_entry* PT = nullptr;
        if (!PD[PD_index].present) {
            //kprintf("new PT\n");
            PT = (page_table_entry*)get_physical_allocator()->allocate();
            memset(PT, 0, 4096);
            PD[PD_index] = get_default_entry();
            PD[PD_index].address = (uint64_t)PT / 4096;
        } else {
            PT = (page_table_entry*) (PD[PD_index].address * 4096);
        }
        //kprintf("PT = 0x%lx\n", PT);

        if (PT[PT_index].present) {
            //kprintf("Virtual Address 0x%lx has already been mapped\n", vaddr);
            return nullptr;
        } else {
            PT[PT_index] = get_default_entry();
            PT[PT_index].address = paddr / 4096;
            PT[PT_index].user = false;
            flush_tlb();
            return &PT[PT_index];
        }

    }

    void PML4E::direct_map_lower_2g() {
        page_table_entry* new_pdpe = (page_table_entry*)get_physical_allocator()->allocate();
        memset(new_pdpe, 0, 4096);
        new_pdpe[0] = get_default_entry();
        new_pdpe[0].address = 0;
        new_pdpe[0].huge_page = 1;

        new_pdpe[1] = get_default_entry();
        new_pdpe[1].address = 1 << 30;
        new_pdpe[1].huge_page = 1;

        this->entries[0].address = (uint64_t )new_pdpe / 4096;
        flush_tlb();
    }

    page_table_entry *PML4E::try_map_user(uint64_t vaddr) {
        auto paddr = get_physical_allocator()->allocate();
        auto ret = try_map(vaddr, paddr);
        ret->global = false;
        ret->user = true;
        flush_tlb();
        return ret;
    }

    void PML4E::load() {
        asm ("movq %0, %%cr3;" :: "r"(this));
    }

    uint64_t get_and_map_multiple(int num) {
        static uint64_t kernel_heap_first_free = KERNEL_HEAP_BASE;
        uint64_t base = __sync_fetch_and_add(&kernel_heap_first_free, num * 4096);
        for (uint64_t addr = base; addr < base + (uint64_t )num * 4096; addr += 4096) {
            uint64_t paddr = get_physical_allocator()->allocate();
            auto res = PML4E::get_current_PML4E()->try_map(addr, paddr);
            assert_true(res);
            //kprintf("Mapped 0x%lx to 0x%lx\n", paddr, addr);
        }
        return base;
    }


    PML4E* create_user_vm () {
        auto new_pml4e = reinterpret_cast<PML4E*>(get_physical_allocator()->allocate());
        memset(new_pml4e, 0, 4096);
        new_pml4e->entries[0] = PML4E::get_current_PML4E()->entries[0];
        new_pml4e->entries[1] = PML4E::get_current_PML4E()->entries[1];
        return new_pml4e;
    }




    void test_page_mapping () {
        for (int i = 0; i < 10000; i++) {
            uint64_t paddr = get_physical_allocator()->allocate();
            PML4E::get_current_PML4E()->try_map( KERNEL_HEAP_BASE + i * 4096, paddr);
            uint64_t * ptr = (uint64_t*)(KERNEL_HEAP_BASE + i * 4096);
            *ptr = 0x23333333;
            assert_true(*ptr == 0x23333333);
        }
        kprintf("page mapping test passed!");
    }

    void test_multi_alloc_map () {
        get_and_map_multiple(5);
    }



}