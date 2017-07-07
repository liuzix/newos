CXX = g++
CFLAGS =  -g -O0 -fno-rtti -nostdlib -fno-exceptions  -mno-red-zone

ASM = nasm
ASMFLAGS = -felf64

ARCH = x86
BUILD_DIR = build


LD = ld
LDFLAGS =  -T src/arch/$(ARCH)/linker.ld -z max-page-size=0x1000


CXX_SOURCES = $(wildcard src/*.cc)
CXX_SOURCES += $(wildcard src/devices/*.cc)
CXX_SOURCES += $(wildcard src/fs/*.cc)
CXX_SOURCES += $(wildcard src/lib/*.cc)

ASM_SOURCES = $(wildcard src/arch/$(ARCH)/*.asm)

CXX_OBJECTS = $(addprefix $(BUILD_DIR)/,$(CXX_SOURCES:.cc=.o))
ASM_OBJECTS = $(addprefix $(BUILD_DIR)/,$(ASM_SOURCES:.asm=.o))

all: kernel.bin

kernel.bin: $(CXX_OBJECTS) $(ASM_OBJECTS)
	$(LD) $(LDFLAGS) $(CXX_OBJECTS) $(ASM_OBJECTS) -o $@

$(CXX_OBJECTS): $(BUILD_DIR)/%.o: %.cc
	mkdir -p $(@D)
	$(CXX) $(CFLAGS) -c $< -o $@

$(ASM_OBJECTS): $(BUILD_DIR)/%.o: %.asm
	mkdir -p $(@D)
	$(ASM) $(ASMFLAGS) $< -o $@

iso: kernel.bin
	mkdir -p isofiles/boot/grub
	cp kernel.bin isofiles/boot/kernel.bin
	cp ramdisk.tar isofiles/boot/ramdisk
	cp grub.cfg isofiles/boot/grub
	grub-mkrescue -o myos.iso isofiles
	rm -r isofiles

clean:
	rm -r build
	rm kernel.bin
	rm myos.iso
	