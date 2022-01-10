.PHONY: build run

build:
	ninja -C build

run:
	qemu-system-i386.exe -kernel build/kernel/kernel -debugcon stdio

