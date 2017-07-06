//
// Created by Zixiong Liu on 6/30/17.
//

#include <elf.h>
#include "userproc.h"
#include "elf.h"
#include "mmu.h"



namespace proc {
    extern "C" void call_ring3 (void(*func)(), uint64_t rsp);


    Userproc::Userproc(const char* executable_path) : Thread(nullptr, executable_path) {
        this->executable = executable_path;
        this->page_dir = memory::create_user_vm();
    }

    int Userproc::call_main_func() {
        // here we load the executable to memory
        ELF_Parser parser(executable);
        while (auto phdr = parser.get_program_header()) {
            if (phdr->p_type != 1) continue;
            assert_true(phdr->p_vaddr >=USER_TEXT_BASE);
            for (uint64_t vaddr = phdr->p_vaddr; vaddr < phdr->p_vaddr + phdr->p_memsz; vaddr += 4096) {
                this->page_dir->try_map_user(vaddr);
                memset((void *)vaddr, 0, 4096);
            }
            parser.fd->file_seek((offset_t )phdr->p_offset);
            parser.fd->file_read((char*)phdr->p_vaddr, (offset_t )phdr->p_filesz);
        }
        auto res = this->page_dir->try_map_user(USER_STACK_BASE - 4096);
        assert(res != nullptr);
        this->user_rsp = USER_STACK_BASE - 16;
        func = (void(*)()) parser.ehdr->e_entry;
        call_ring3(func, user_rsp);
        return 0;
    }

    void Userproc::will_switch() {
        assert_true(this->page_dir != nullptr);
        this->page_dir->load();
    }

    Thread *Userproc::new_userprog(const char *path) {
        Thread* ret = new Userproc(path);
        scheduler->add_thread(ret);
        return ret;
    }

    void Userproc::exit(uint64_t error_code) {
        // release pages etc.
        Thread::exit(error_code);
    }

    void sys_exit(uint64_t error_code) {
        auto cur = static_cast<Userproc*>(get_current());
        if (cur == nullptr) {
            assert(!"sys_exit on null thread!");
            return;
        }
        cur->exit(error_code);
    }


}