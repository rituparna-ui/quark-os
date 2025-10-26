#! /bin/bash

aarch64-linux-gnu-as src/boot.S -o build/boot.o
aarch64-linux-gnu-gcc -ffreestanding -c src/kernel.c -o build/kernel.o
aarch64-linux-gnu-ld -nostdlib -T linker.ld build/boot.o build/kernel.o -o build/kernel.elf

qemu-system-aarch64 -machine virt -nographic -cpu cortex-a72 -kernel build/kernel.elf
# qemu-system-aarch64 -machine virt -nographic -cpu cortex-a72 -kernel build/kernel.elf -s -S
# qemu-system-aarch64 -machine virt,virtualization=on,secure=on -nographic -cpu cortex-a72 -kernel build/kernel.elf
