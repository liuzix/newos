#include "multiboot.h"
#include "lib/printf.h"

namespace multiboot {

    intptr_t memory_base = 0;
    intptr_t memory_length = 0;

    intptr_t last_section = 0;
    intptr_t last_section_length = 0;

    static uint64_t highest_used = 0;

    uint64_t ramdisk_start, ramdisk_end;
    static void update_highest_used(uint64_t addr) {
        if (addr > highest_used)
            highest_used = addr;
    }

    intptr_t get_first_free_page () {
        return (highest_used + 4095) & ((-1) << 12);
    }



    void parse_tags(intptr_t addr) {
        if (addr % 8 != 0) {
            kprintf("Unaligned mbi: 0x%x\n", addr);
        }

        for (multiboot_tag *tag = (multiboot_tag *) (addr + 8);
             tag->type != 0; tag = (multiboot_tag *) ((intptr_t) tag + ((tag->size + 7) & ~7))) {
            if (tag->type == 6) {
                for (multiboot_mmap_entry *mmap = ((multiboot_tag_mmap *) tag)->entries;
                     (intptr_t) mmap < (intptr_t) tag + tag->size;
                     mmap = (multiboot_mmap_entry *) ((intptr_t) mmap + ((multiboot_tag_mmap *) tag)->entry_size)) {

                    kprintf(" base_addr = 0x%x%x,"
                                    " length = 0x%x%x, type = 0x%x\n",
                            (unsigned) (mmap->addr >> 32),
                            (unsigned) (mmap->addr & 0xffffffff),
                            (unsigned) (mmap->len >> 32),
                            (unsigned) (mmap->len & 0xffffffff),
                            (unsigned) mmap->type);

                    if (mmap->addr >= 0x100000 && mmap->type == 1) {
                        memory_base = mmap->addr;
                        memory_length = mmap->len;
                    }
                }
            }

            if (tag->type == 9) {
                multiboot_tag_elf *elf_tag = (multiboot_tag_elf *) tag;
                for (int i = 0; i < elf_tag->num; i++) {
                    uintptr_t add = elf_tag->headers[i].sh_addr;
                    uintptr_t sec_len = elf_tag->headers[i].sh_size;
                    //kprintf("section addr = 0x%lx, size = %ld\n", add, sec_len);
                    if (last_section < add) {
                        last_section = add;
                        last_section_length = sec_len;
                    }
                    update_highest_used(add + sec_len);
                }
            }

            if (tag->type == 3) {
                multiboot_tag_module *mod_tag = reinterpret_cast<multiboot_tag_module*>(tag);
                kprintf("module start = 0x%x, end = 0x%x, name = %s\n",
                    mod_tag->mod_start, mod_tag->mod_end, mod_tag->str);
                update_highest_used(mod_tag->mod_end);
                ramdisk_start = mod_tag->mod_start;
                ramdisk_end = mod_tag->mod_end;
            }
        }

        kprintf("first free page = 0x%lx\n", get_first_free_page());
    }

}