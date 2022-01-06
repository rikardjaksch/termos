#include <kernel/io.h>

namespace io {
  uint8_t in8(uint16_t port) {
    uint8_t value;
    asm("inb %%dx, %%al":"=a"(value):"d"(port));
    return value;
  }

  uint16_t in16(uint16_t port) {
    uint16_t value;
    asm("inw %%dx, %%ax":"=a"(value):"d"(port));
    return value;
  }

  uint32_t in32(uint16_t port) {
    uint32_t value;
    asm("inl %%dx, %%eax":"=a"(value):"d"(port));
    return value;
  }

  void out8(uint16_t port, uint8_t value) {
    asm("outb %%al, %%dx"::"d"(port), "a"(value));
  }

  void out16(uint16_t port, uint16_t value) {
    asm("outw %%ax, %%dx"::"d"(port), "a"(value));
  }

  void out32(uint16_t port, uint32_t value) {
    asm("outl %%eax, %%dx"::"d"(port), "a"(value));
  }
}
