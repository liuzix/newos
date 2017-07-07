//
// Created by Zixiong Liu on 6/21/17.
//

#include "gdt.h"
#include "mmu.h"
#include "lib/printf.h"
#include "lib/assert.h"
#include "lock.h"
#include "lib/cpu_local.h"

#define GDT_LIMIT 4095

extern "C" void reload_segment();

static void* gdt_pointer = nullptr;

static synchronization::SpinLock gdt_lock;

static struct {
    uint16_t limit;
    uint64_t addr;
} __attribute__((packed)) GDTR ;

static inline void write_to_gdt(void* descriptor, uint64_t length, int index) {
    if (length != 8 && length != 16) {
        kprintf("Illegal descriptor length\n");
        assert_true(false);
    }
    if (index % 8 != 0) {
        kprintf("Illegal index\n");
        assert_true(false);
    }
    assert_true(gdt_pointer != nullptr);
    memcpy((char*)gdt_pointer + index, descriptor, length);
}

enum Segment_type {data, code, tss};
static inline dscr_64 get_mem_segment(Segment_type type, int privilege) {
    if (type != data && type != code) {
        assert_true(false);
    }
    //dscr_64 ret = {.base_lo = 0, .base_mid = 0, .base_hi = 0, .limit = 0,
    //               .attrs = 0};
    dscr_64 ret = {
        limit: 0,
        base_lo: 0,
        base_mid: 0,
        attrs: 0,
        base_hi: 0,
    };
    ret.attrs |= 1 << 1; // readable or writable
    ret.attrs |= (uint16_t)(type == code ? 0b11 : 0b10) << 3; // type
    ret.attrs |= privilege << 5;
    ret.attrs |= 1 << 7;  // Present
    if (type == code) {
        ret.attrs |= 1 << 13; // Long mode
    }
    return ret;
}

static inline dscr_128 get_tss_segment(Segment_type type, tss_t* addr) {
    if (type != tss) assert_true(false);
    auto addr_int = reinterpret_cast<uint64_t >(addr);
    dscr_128 ret;
    ret.base_lo = (uint16_t )addr_int & 0xFFFF;
    ret.base_mid = (addr_int >> 16) & 0xFF;
    ret.base_hi = (addr_int >> 24) & 0xFF;
    ret.base_uhi = (addr_int >> 32) & 0xFFFFFFFF;
    ret.limit = sizeof(tss_t) - 1;
    ret.attrs = 0;
    ret.reseved = 0;
    ret.attrs = 0b1001;
    ret.attrs |= 1 << 7;
    return ret;
}

static CPU_local<tss_t*> *tss_per_cpu;

extern "C" void set_up_ring0_rsp (uint64_t rsp) {
    tss_per_cpu->get()->rsp0 = rsp & 0xFFFFFFFFFFFFFFF0;
}

uint64_t get_ring0_rsp () {
    return tss_per_cpu->get()->rsp0;
}

void create_and_set_tss() {
    gdt_lock.lock();
    if (tss_per_cpu == nullptr) {
        tss_per_cpu = new CPU_local<tss_t*>;
    }
    gdt_lock.unlock();
    auto tss_ptr = reinterpret_cast<tss_t*>(memory::get_and_map_multiple(1));
    tss_per_cpu->set(tss_ptr);
    auto tss_segment = get_tss_segment(tss, tss_ptr);
    auto tss_index = 40 + get_cpu_id() * 16;
    write_to_gdt(&tss_segment, 16, tss_index);
    asm ("ltr %0" :: "m"(tss_index));

}

void init_gdt() {
    gdt_lock.lock();
    if (gdt_pointer == nullptr) {
        gdt_pointer = (void *) memory::get_and_map_multiple(1);
        memset(gdt_pointer, 0, 4096);
        dscr_64 code_0 = get_mem_segment(code, 0);
        write_to_gdt(&code_0, 8, 8);
        dscr_64 data_0 = get_mem_segment(data, 0);
        write_to_gdt(&data_0, 8, 16);
        dscr_64 code_3 = get_mem_segment(code, 3);
        write_to_gdt(&code_3, 8, 32);
        dscr_64 data_3 = get_mem_segment(data, 3);
        write_to_gdt(&data_3, 8, 24);

        GDTR.limit = GDT_LIMIT;
        GDTR.addr = (uint64_t) gdt_pointer;
    }
    asm ("lgdt %0" :: "m"(GDTR));
    reload_segment();

    gdt_lock.unlock();
}


