//
// Created by Zixiong Liu on 6/21/17.
//

#ifndef MYOS_GDT_H
#define MYOS_GDT_H

#include <cstdint>

struct dscr_64 {
    uint16_t limit;
    uint16_t base_lo;
    uint8_t base_mid;
    uint16_t attrs;
    uint8_t base_hi;
} __attribute__((packed));

struct dscr_128 {
    uint16_t limit;
    uint16_t base_lo;
    uint8_t base_mid;
    uint16_t attrs;
    uint8_t base_hi;
    uint32_t base_uhi;
    uint32_t reseved;
} __attribute__((packed));

struct tss_t {
    uint32_t reserved1;
    uint64_t rsp0;
    uint64_t rsp1;
    uint64_t rsp2;
    uint64_t reserved2;
    uint64_t ist1, ist2, ist3, ist4, ist5, ist6, ist7;
    uint64_t reserved3;
    uint16_t reserved4;
    uint16_t iomap;
} __attribute__((packed));

void init_gdt();
void create_and_set_tss();
uint64_t get_ring0_rsp ();

#endif //MYOS_GDT_H
