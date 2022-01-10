set(KERNEL_SOURCES
    boot/boot.S
    kmain.c
    kprintf.c
    vga.c
    io.c
    i386.c
    i386.S)

set(KLIB_SOURCES
    klibc/stdlib.c)

set(SOURCES
    ${KERNEL_SOURCES}
    ${KLIB_SOURCES}
)

add_executable(kernel ${SOURCES})

target_include_directories(kernel PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_options(kernel PUBLIC "-DKERNEL")
target_compile_options(kernel PUBLIC "-std=gnu11")
target_compile_options(kernel PUBLIC "-ffreestanding")
target_compile_options(kernel PUBLIC "-fbuiltin")
target_compile_options(kernel PUBLIC "-nostdinc")
target_compile_options(kernel PUBLIC "-nostdlib")
target_compile_options(kernel PUBLIC "-Wextra")
target_compile_options(kernel PUBLIC "-Wall")

target_compile_options(kernel PUBLIC $<$<COMPILE_LANGUAGE:ASM>:-DASM_FILE>)

target_link_options(kernel PUBLIC -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
target_link_options(kernel PUBLIC -nostdlib)
target_link_options(kernel PUBLIC -ffreestanding)
target_link_options(kernel PUBLIC -O2)

target_link_libraries(kernel gcc)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKERNEL")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie -fPIE -ffreestanding -fbuiltin")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -nostdinc -nostdinc++")

#add_link_options(LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib)



