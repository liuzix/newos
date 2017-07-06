//
// Created by Zixiong Liu on 6/26/17.
//

#ifndef MYOS_LINKEDLIST_H
#define MYOS_LINKEDLIST_H
#include <iterator>

template <class T>
class LinkedlistIterator;

template <class T>
class LinkedList {
    T* head;
public:
    typedef LinkedlistIterator<T> iterator;

    LinkedList() : head (nullptr) {}

    void push_front(T* obj) {
        obj->next = head;
        head = obj;
    }

    T* pop_front() {
        auto ret = head;
        head = head->next;
        return ret;
    }

    iterator begin(){
        return iterator(head);
    }

    iterator end() {
        return iterator(nullptr);
    }

    void remove (iterator& iter) {
        if (iter.former) {
            iter.former->next = iter.current->next;
        } else {
            head = iter.current->next;
        }
        iter.current = iter.current->next;
    }

};

template <class T>
class LinkedlistIterator : public std::iterator<std::forward_iterator_tag, T> {
    friend class LinkedList<T>;
public:
    explicit LinkedlistIterator(T* _ptr) : current(_ptr), former(nullptr) { }
    T* operator*() {
        return current;
    }

    T* operator->() const {
        return current;
    }

    LinkedlistIterator<T> & operator++ () {
        former = current;
        if (current)
            current = current->next;
        return *this;
    }

    LinkedlistIterator<T> operator++(int) {
        LinkedlistIterator<T> old(*this);
        former = current;
        if (current)
            current = current->next;
        return old;
    }

    bool operator!=(const LinkedlistIterator<T> &other) const {
        return this->current != other.current;
    }

private:
    T* former;
    T* current;
};

////

struct testing_dummy {
    testing_dummy* next;
    int value;
    testing_dummy (int i) : value (i) {}
};





#endif //MYOS_LINKEDLIST_H
