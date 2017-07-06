//
// Created by Zixiong Liu on 7/1/17.
//

#include <elf.h>
#include "elf.h"
#include "fs/vfs.h"

static bool check_is_elf(const Elf64_Ehdr& header) {
    if (header.e_ident[0] == 0x7f
            && header.e_ident[1] == 'E'
            && header.e_ident[2] == 'L'
            && header.e_ident[3] == 'F') {
        return true;
    } else {
        return false;
    }
}

ELF_Parser::ELF_Parser(const char *path) {
    fd = global_file_open(path);
    ehdr = new Elf64_Ehdr;
    fd->file_read((char*)ehdr, sizeof(Elf64_Ehdr));
    assert (check_is_elf(*ehdr));
    kprintf("ELF check passed\n");
    phdr = new Elf64_Phdr[ehdr->e_phnum];
    assert (ehdr->e_phentsize == sizeof (Elf64_Phdr));
    phnum = ehdr->e_phnum;
    fd->file_seek((offset_t)ehdr->e_phoff);
    fd->file_read((char*)phdr, sizeof(Elf64_Phdr) * phnum);
}

const Elf64_Phdr *ELF_Parser::get_program_header() {
    if (cur > phnum) return nullptr;
    return &phdr[cur++];
}
