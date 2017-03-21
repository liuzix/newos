#pragma once
#include <stdint.h>

namespace multiboot {
	extern intptr_t memory_base;
	extern intptr_t memory_length;

	struct multiboot_tag {
		uint32_t type;
		uint32_t size;
	};

	struct multiboot_mmap_entry {
		uint64_t addr;
		uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
		uint32_t type;
		uint32_t zero;
	} __attribute__((packed));

	struct multiboot_tag_mmap
	{
		uint32_t type;
		uint32_t size;
		uint32_t entry_size;
		uint32_t entry_version;
		struct multiboot_mmap_entry entries[0];
	} __attribute__((packed));

	typedef struct elf64_shdr {
		uint32_t sh_name;		/* Section name, index in string tbl */
		uint32_t sh_type;		/* Type of section */
		uint64_t sh_flags;		/* Miscellaneous section attributes */
		uint64_t sh_addr;		/* Section virtual addr at execution */
		uint64_t sh_offset;		/* Section file offset */
		uint64_t sh_size;		/* Size of section in bytes */
		uint32_t sh_link;		/* Index of another section */
		uint32_t sh_info;		/* Additional section information */
		uint64_t sh_addralign;	/* Section alignment */
		uint64_t sh_entsize;	/* Entry size if section holds table */
	} Elf64_Shdr;

	struct multiboot_tag_elf {
		uint32_t type;
		uint32_t size;
		uint16_t num;
		uint16_t entsize;
		uint16_t shndx;
		uint16_t reserved;
		elf64_shdr headers[];
	} __attribute__((packed));


	void parse_tags(intptr_t addr);
}
