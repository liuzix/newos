//
// Created by Zixiong Liu on 6/25/17.
//

#ifndef MYOS_LIST_H
#define MYOS_LIST_H

#include "assert.h"

template <class T>
class Queue {
    T* head;
    T* tail;
public:

    void enqueue(T* obj) {
        if (head == nullptr)
            head = obj;
        if (tail) {
            tail->next = obj;
        }
        tail = obj;
        obj->next = nullptr;
    }

    T* dequeue () {
        if (!head)
            return nullptr;
        T* ret = head;
        head = head->next;
        if (ret == tail) {
            tail = nullptr;
            assert_true(head == nullptr);
        }
        return ret;
    }

};

#endif //MYOS_LIST_H
