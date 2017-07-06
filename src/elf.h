//
// Created by Zixiong Liu on 7/1/17.
//

#ifndef MYOS_ELF_H
#define MYOS_ELF_H

# include <stdint.h>
# include <elf.h>
#include "fs/vfs.h"

class ELF_Parser {
private:
    Elf64_Phdr* phdr;

    uint64_t phnum;
    uint64_t cur {0};
public:
    ELF_Parser (const char* path);
    const Elf64_Phdr* get_program_header();
    FileHandle* fd {nullptr};
    Elf64_Ehdr* ehdr {nullptr};
};



#endif //MYOS_ELF_H
