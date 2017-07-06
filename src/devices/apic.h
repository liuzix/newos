//
// Created by Zixiong Liu on 6/18/17.
//

#ifndef MYOS_APIC_H
#define MYOS_APIC_H

#include <stdint.h>
#define TIMER_INTERRUPT_VECTOR 32
#define TIMER_INTERVAL 1

void init_apic ();
void write_apic_register_64(uint32_t offset, uint64_t value) ;
uint64_t read_apic_register (uint32_t offset);
void timed_delay (uint32_t interval);
uint64_t get_local_apic_id ();
void enable_timer ();
void apic_eoi();
void global_init_apic();



#endif //MYOS_APIC_H
