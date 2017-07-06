//
// Created by Zixiong Liu on 6/23/17.
//

#ifndef MYOS_THREAD_H
#define MYOS_THREAD_H

#include <cstdint>
#include "lib/cpu_local.h"
#include "lock.h"
#include "lib/queue.h"
#include "lib/linkedlist.h"

using namespace synchronization;
namespace proc {

    enum ThreadStatus {running, runnable, asleep, dead};
    struct Thread {
        uint64_t rsp;
        Thread* next;
        ThreadStatus status;
        const char* name;
        int64_t time_to_switch;
        int64_t time_to_wakeup;

        ///
        void yield();
        void sleep(uint64_t ms);
        ///
        static Thread* new_thread(void (*_func)(), char* name);
        //static void thread_returned();
        virtual void will_switch();
        virtual void exit(uint64_t error_code);
    private:
        static void thread_entrance(Thread* t);

        // other kinds of thread will have to override these two
        virtual int call_main_func();
    protected:
        Thread(void (*_func) (), const char* _name);

        void (*func) ();
    };

    class Scheduler {
        Queue<Thread> runnable_queue;
        LinkedList <Thread> sleep_list;
        SpinLock lock;
        SpinLock sleep_list_lock;
    public:
        void schedule_loop();
        void add_thread(Thread* thrd);
        void sleep_thread(Thread* thrd);
        void refresh_sleep_list();
        CPU_local<Thread*> current;
        CPU_local<uint64_t> loop_rsp;
    };

    extern Scheduler * volatile scheduler;

    void scheduler_init();
    void enter_schedule_loop();
    void thread_tick();

    inline void push_rsp(uint64_t* rsp, uint64_t value) {
        *rsp -= 8;
        *(uint64_t *)*rsp = value;
    }

    inline Thread* get_current() {
        return scheduler->current.get();
    }
}

#endif //MYOS_THREAD_H
