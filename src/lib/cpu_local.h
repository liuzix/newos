//
// Created by Zixiong Liu on 6/23/17.
//

#ifndef MYOS_CPU_LOCAL_H
#define MYOS_CPU_LOCAL_H

#include "../smp.h"
#include "assert.h"

template <class T>
class CPU_local {
    T* data;
    bool* valid;
public:
    CPU_local() {
        int cpu_num = get_num_cpu();
        data = new T[cpu_num];
        valid = new bool[cpu_num];
        for (int i = 0; i < cpu_num; i++)
            valid[i] = false;
    }

    T get() {
        int cpu_id = get_cpu_id();
        assert_true(valid[cpu_id]);
        return data[cpu_id];
    }

    void set (const T x) {
        int cpu_id = get_cpu_id();
        data[cpu_id] = x;
        valid[cpu_id] = true;
    }

    T* get_ptr() {
        int cpu_id = get_cpu_id();
        valid[cpu_id] = true;
        return &data[cpu_id];
    }
};

#endif //MYOS_CPU_LOCAL_H
