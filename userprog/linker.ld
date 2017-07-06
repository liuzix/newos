SECTIONS
{
    . = 0x10000000000;
    .text : { KEEP(*(.text)) }
    .rodata : { KEEP(*(.rodata*)) }
    .data :  { KEEP(*(.data)) }
    .bss :  { KEEP(*(.bss)) }
}
