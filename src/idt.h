//
// Created by Zixiong Liu on 6/19/17.
//

#ifndef MYOS_IDT_H
#define MYOS_IDT_H

#include <stdint.h>

struct IDTEntry {
    uint16_t offset_1; // offset bits 0..15
    uint16_t selector; // a code segment selector in GDT or LDT
    uint8_t ist;       // bits 0..2 holds Interrupt Stack Table offset, rest of bits zero.
    uint8_t type_attr; // type and attributes
    uint16_t offset_2; // offset bits 16..31
    uint32_t offset_3; // offset bits 32..63
    uint32_t zero;     // reserved
} __attribute__((packed));

void init_idt();
void register_handler(int vec_no, void (*func)());


#endif //MYOS_IDT_H
