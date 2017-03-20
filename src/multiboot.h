#pragma once
#include <stdint.h>

namespace multiboot {
	extern uint8_t memory_base;
	extern uint8_t memory_length;

	struct multiboot_tag {
		uint32_t type;
		uint32_t size;
	};	struct multiboot_mmap_entry {
		uint64_t addr;
		uint64_t len;
#define MULTIBOOT_MEMORY_AVAILABLE 1
#define MULTIBOOT_MEMORY_RESERVED 2
#define MULTIBOOT_MEMORY_ACPI_RECLAIMABLE 3
#define MULTIBOOT_MEMORY_NVS 4
		uint32_t type;
		uint32_t zero;
	} __attribute__((packed));	struct multiboot_tag_mmap
	{
		uint32_t type;
		uint32_t size;
		uint32_t entry_size;
		uint32_t entry_version;
		struct multiboot_mmap_entry entries[0];
	};	void parse_tags(intptr_t addr);}