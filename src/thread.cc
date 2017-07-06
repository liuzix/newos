//
// Created by Zixiong Liu on 6/23/17.
//

#include "thread.h"
#include "mmu.h"
#include "x86.h"
#include "devices/apic.h"

namespace proc {
    Scheduler * volatile scheduler;

    extern "C" void switch_context(uint64_t* prev_rsp, uint64_t* next_rsp);
    static void recycle_thread(Thread* thread) {
        delete thread;
    }

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
    void Scheduler::schedule_loop() {
        Thread* next_thread = nullptr;
        current.set(nullptr);
        enable_timer();
        for (;;) {

            // insert current thread
            auto current_thread = current.get();
            current.set(nullptr);
            asm ("sti");

            if (!current_thread) goto no_current;
            if (current_thread->status == runnable) {
                lock.lock();
                add_thread(current_thread);
                current_thread->status = runnable;
                lock.unlock();
            } else if (current_thread->status == dead) {
                recycle_thread(current_thread);
            }

            no_current:

            for (;;) {
                lock.lock();
                next_thread = runnable_queue.dequeue();
                if (next_thread) {
                    lock.unlock();
                    break;
                }
                lock.unlock(); // give another cpu chance to add thread
                asm ("sti; hlt; ");
            }
            assert_true(next_thread->status == runnable);
            next_thread->will_switch();
            current.set(next_thread);
            switch_context(loop_rsp.get_ptr(), &next_thread->rsp);
        }
    }
#pragma clang diagnostic pop

    void Scheduler::add_thread(Thread *thrd) {
        bool int_enabled = x86::get_interrupt_status();
        bool locked = lock.trylock();
        asm ("cli");
        thrd->status = runnable;
        runnable_queue.enqueue(thrd);
        if (locked) lock.unlock();
        if (int_enabled)
                asm ("sti");
    }

    void Scheduler::sleep_thread(Thread *thrd)  {
        bool int_enabled = x86::get_interrupt_status();
        x86::cli();
        sleep_list_lock.lock();
        sleep_list.push_front(thrd);
        sleep_list_lock.unlock();
        if (int_enabled)
            x86::sti();
    }

    void Scheduler::refresh_sleep_list() {
        sleep_list_lock.lock();
        for (auto iter = sleep_list.begin(); iter != sleep_list.end(); iter++) {
            iter->time_to_wakeup --;
            if (iter->time_to_wakeup <= 0) {
                auto thrd = *iter;
                sleep_list.remove(iter);
                add_thread(thrd);
            }
        }
        sleep_list_lock.unlock();
    }



    Thread::Thread(void (*_func) (), const char* _name) {
        func = _func;
        rsp = memory::get_and_map_multiple(1) + 4096 - 16;
        status = runnable;
        next = nullptr;
        name = _name;
        time_to_switch = 25;
        push_rsp(&rsp, 0);
        push_rsp(&rsp, (uint64_t)&Thread::thread_entrance);
        rsp -= 6 * 8; // some empty registers
        push_rsp(&rsp, (uint64_t)this);
        push_rsp(&rsp, 0);
    }


    Thread *Thread::new_thread(void (*_func) (), char* _name) {
        Thread* ret = new Thread(_func, _name);
        scheduler->add_thread(ret);
        return ret;
    }

    void Thread::exit(uint64_t error_code) {
        kprintf("Thread %s has returned\n", name);
        this->status = dead;
        yield();
        // free thread resources. KEEP the kernel stack
    }

    void Thread::yield() {
        time_to_switch = 25;
        asm ("cli");
        switch_context(&this->rsp, scheduler->loop_rsp.get_ptr());
        asm ("sti");
    }

    void Thread::thread_entrance(Thread* t) {
        asm ("sti");
        t->call_main_func();
        t->exit(0);
    }

    void Thread::sleep(uint64_t ms) {
        time_to_wakeup = ms;
        scheduler->sleep_thread(this);
        this->status = asleep;
        yield();
    }

    int Thread::call_main_func() {
        this->func();
        return 0;
    }

    void Thread::will_switch() {
        // nothing here
    }


    void scheduler_init() {
        scheduler = new Scheduler;
    }

    void enter_schedule_loop() {
        while (!scheduler);
        scheduler->schedule_loop();
    }

    void thread_tick() {
        assert_true(!x86::get_interrupt_status());
        if (proc::scheduler) {
            if (get_cpu_id() == 0)
                scheduler->refresh_sleep_list();
            auto cur = scheduler->current.get();
            if (cur) {
                cur->time_to_switch -= 1;
                if (cur->time_to_switch <= 0)
                    cur->yield();
            }
        }
    }


}