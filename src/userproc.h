//
// Created by Zixiong Liu on 6/30/17.
//

#ifndef MYOS_USERPROC_H
#define MYOS_USERPROC_H

#include "thread.h"
#include "mmu.h"

using namespace memory;
namespace proc {
    struct Userproc : public Thread {
    public:
        static Thread* new_userprog(const char* path);
        void exit(uint64_t error_code) override ;
    private:

        const char* executable {nullptr};
        PML4E* page_dir {nullptr};
        uint64_t user_rsp {0};
        ///
        Userproc (const char* executable_path);
        int call_main_func() override ;
        void will_switch() override ;
    };

    void sys_exit(uint64_t error_code);
}

#endif //MYOS_USERPROC_H
