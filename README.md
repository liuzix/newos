# newos

This is an OS kernel written for fun by myself. It's not complete and probably never will. But is nonetheless a good framework to build on.

## what it does
1. It detects all the memory and other hardware information by multiboot2 specifications.

2. Kernel heap is completely bug free and functional, and is thread-safe.

3. It supports a memory file system, based on tar format. A tar file can be loaded as a module by a multiboot compatible bootloader.

4. It includes an AHCI driver, which will load automatically on detection of an AHCI controller. The driver supports reading/writing by blocks.

5. Virtual file system interface is under development. It will support on disk file system and memory file system.

6. Simple support of user threads and system calls.

## how to compile and run it

make; ./iso.sh

You can then boot it by a virtual machine.
