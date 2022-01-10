.PHONY: build run

build:
	ninja -C build

run: build
	qemu-system-i386.exe \
	-kernel build/kernel/kernel \
	-debugcon stdio \
	-m 256M \
	-cpu max \
	-device VGA,vgamem_mb=64

