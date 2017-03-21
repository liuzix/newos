#include "multiboot.h"
#include "lib/printf.h"

namespace multiboot {

	intptr_t memory_base;
	intptr_t memory_length;


	void parse_tags(intptr_t addr)
	{
		if (addr % 8 != 0) {
			kprintf("Unaligned mbi: 0x%x\n", addr);
		}

		for (multiboot_tag* tag = (multiboot_tag*)(addr + 8);
			tag->type != 0; tag = (multiboot_tag*)((intptr_t)tag + ((tag->size + 7) & ~7))) {
			if (tag->type == 6) {
				for (multiboot_mmap_entry* mmap = ((multiboot_tag_mmap*)tag)->entries;
					(intptr_t)mmap < (intptr_t)tag + tag->size;
					mmap = (multiboot_mmap_entry*)((intptr_t)mmap + ((multiboot_tag_mmap*)tag)->entry_size)) {

					kprintf(" base_addr = 0x%x%x,"
						" length = 0x%x%x, type = 0x%x\n",
						(unsigned)(mmap->addr >> 32),
						(unsigned)(mmap->addr & 0xffffffff),
						(unsigned)(mmap->len >> 32),
						(unsigned)(mmap->len & 0xffffffff),
						(unsigned)mmap->type);

					if (mmap->addr >= 0x100000 && mmap->type == 1) {
						memory_base = mmap->addr;
						memory_length = mmap->len;
					}
				}
			}

			if (tag->type == 9) {
				multiboot_tag_elf* elf_tag = (multiboot_tag_elf*)tag;
				for (int i = 0; i < elf_tag->num; i++) {
					intptr_t addr = elf_tag->headers[i].sh_addr;
					intptr_t sec_len = elf_tag->headers[i].sh_size;
					kprintf("section addr = 0x%lx, size = %ld\n");
				}
			}
		}
	}

}