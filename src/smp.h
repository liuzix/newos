//
// Created by Zixiong Liu on 6/17/17.
//

#ifndef MYOS_SMP_H
#define MYOS_SMP_H
void mpboot_init ();
void register_cpu();

int get_cpu_id();
int get_num_cpu();

#endif //MYOS_SMP_H
