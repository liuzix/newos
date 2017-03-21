cmake_minimum_required(VERSION 3.5)
project(myos CXX ASM_NASM)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_SYSTEM_NAME  Myos)
MESSAGE(STATUS "Setting toochain...")
#set(CMAKE_C_COMPILER /Users/zixiong/x-tools/x86_64-unknown-elf/bin/x86_64-unknown-elf-gcc)
#set(CMAKE_CXX_COMPILER /Users/zixiong/x-tools/x86_64-unknown-elf/bin/x86_64-unknown-elf-g++)

#include_directories( ${CMAKE_CURRENT_SOURCE_DIR})

MESSAGE(STATUS "Adding arch-dependent files")

ENABLE_LANGUAGE(ASM_NASM)
SET(CMAKE_ASM_NASM_COMPILE_OBJECT
        "<CMAKE_ASM_NASM_COMPILER>  -felf64 -o <OBJECT> <SOURCE>")

file(GLOB ARCH_SOURCE_FILES src/boot/*.asm)
file(GLOB MAIN_SOURCE_FILES src/*.cc)
file(GLOB LIB_SOURCE_FILES src/lib/*.cc)
file(GLOB DEV_SOURCE_FILES src/devices/*.cc)

#set(CMAKE_CXX_LINK_EXECUTABLE "/usr/local/binutils/bin/x86_64-unknown-linux-gnu-ld")
set(CMAKE_CXX_FLAGS "-g -O0 -nostdlib -fno-exceptions  -mno-red-zone")
#src/crti.o src/crtbegin.o src/crtend.o src/crtn.o
add_executable(bootloader ${ARCH_SOURCE_FILES} ${MAIN_SOURCE_FILES} ${LIB_SOURCE_FILES} ${DEV_SOURCE_FILES})
set_target_properties(bootloader PROPERTIES LINK_FLAGS  "-fuse-ld=gold  -Wl,-T ${CMAKE_CURRENT_SOURCE_DIR}/src/boot/linker.ld" LINKER_LANGUAGE CXX)



#add_executable(myos ${SOURCE_FILES})
