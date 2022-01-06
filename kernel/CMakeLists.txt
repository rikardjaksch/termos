set(KERNEL_SOURCES
    boot/boot.S
    entry_point.cpp
    vga.cpp)

set(SOURCES
    ${KERNEL_SOURCES}
)

add_executable(kernel ${SOURCES})

target_include_directories(kernel PUBLIC ${CMAKE_SOURCE_DIR})

target_compile_options(kernel PUBLIC "-DKERNEL")
target_compile_options(kernel PUBLIC "-ffreestanding")
target_compile_options(kernel PUBLIC "-fno-exceptions")
target_compile_options(kernel PUBLIC "-fno-rtti")

target_link_options(kernel PUBLIC -T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld)
target_link_options(kernel PUBLIC -nostdlib)

target_link_libraries(kernel gcc)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DKERNEL")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pie -fPIE -ffreestanding -fbuiltin")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -nostdlib -nostdinc -nostdinc++")

#add_link_options(LINKER:-T ${CMAKE_CURRENT_SOURCE_DIR}/linker.ld -nostdlib)



