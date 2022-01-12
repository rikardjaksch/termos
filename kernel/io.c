#include <kernel/io.h>

uint8_t io_in8(uint16_t port) {
  uint8_t value;
  asm("inb %%dx, %%al":"=a"(value):"d"(port));
  return value;
}

uint16_t io_in16(uint16_t port) {
  uint16_t value;
  asm("inw %%dx, %%ax":"=a"(value):"d"(port));
  return value;
}

uint32_t io_in32(uint16_t port) {
  uint32_t value;
  asm("inl %%dx, %%eax":"=a"(value):"d"(port));
  return value;
}

void io_out8(uint16_t port, uint8_t value) {
  asm("outb %%al, %%dx"::"d"(port), "a"(value));
}

void io_out16(uint16_t port, uint16_t value) {
  asm("outw %%ax, %%dx"::"d"(port), "a"(value));
}

void io_out32(uint16_t port, uint32_t value) {
  asm("outl %%eax, %%dx"::"d"(port), "a"(value));
}

void io_wait() {
    io_in8(0x80);
}