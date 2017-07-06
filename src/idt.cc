//
// Created by Zixiong Liu on 6/19/17.
//

#include "idt.h"
#include "lib/printf.h"
#include <boost/preprocessor/repetition/repeat.hpp>
#include <cstring>

IDTEntry IDT[256];

static void (*handlers[256])();

static struct {
    uint16_t length;
    void*    base;
} __attribute__((packed)) IDTR;

static inline void lidt(void* base, uint16_t size) {   // This function works in 32 and 64bit mode
    IDTR = { size, base };
    asm ( "lidt %0" : : "m"(IDTR) );  // let the compiler choose an addressing mode
}

struct interrupt_frame {

} __attribute__((packed));

extern "C" void do_irq(int vec_no) {
    ///

    if (handlers[vec_no] == nullptr) {
        kprintf("Interrupt vector %d\n", vec_no);
    }
    else
        handlers[vec_no]();
}

#define DEFINE_HANDLER(z, n, text) extern "C" void intr_handler_ ## n ();



#define REGISTER_HANDLER(z, n, text) create_idt_entry(IDT + n, (void*)intr_handler_ ## n);



extern "C" void GP_interrupt_handler () {
    kprintf("GP fault!\n");
    asm ("cli; hlt");
}

extern "C" void page_interrupt_handler () {
    kprintf("page fault!\n");
    asm ("cli; hlt");
}

extern "C"  void double_interrupt_handler () {
    kprintf("double fault!\n");
    asm ("cli; hlt");
}


#define IDT_TYPE_INTERRUPT 0xE
#define IDT_DPL_RING0 0
#define IDT_PRESENT (1 << 7)

static void create_idt_entry(IDTEntry* entry, void* handler) {
    entry->ist = 0;
    entry->offset_1 = (uint16_t )(uint64_t) handler;
    entry->offset_2 = (uint16_t )((uint64_t) handler >> 16);
    entry->offset_3 = (uint32_t )((uint64_t) handler >> 32);
    entry->selector = 0x8;
    entry->zero = 0;
    entry->type_attr = IDT_TYPE_INTERRUPT | IDT_DPL_RING0 | IDT_PRESENT;
}


extern "C" void page_fault_stub();
extern "C" void GP_fault_stub();
BOOST_PP_REPEAT(255, DEFINE_HANDLER, IGN)

void init_idt() {
    static bool initialized = false;
    if (!initialized) {
        initialized = true;
        BOOST_PP_REPEAT(255, REGISTER_HANDLER, IGN)
        create_idt_entry(IDT + 13, (void *) GP_fault_stub);
        create_idt_entry(IDT + 14, (void *) page_fault_stub);
        create_idt_entry(IDT + 8, (void *) double_interrupt_handler);
        memset(handlers, 0, sizeof(handlers));
    }
    lidt(IDT, 256 * 16 - 1);
}

void register_handler(int vec_no, void (*func)()) {
    handlers[vec_no] = func;
}