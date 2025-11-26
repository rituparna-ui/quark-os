CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy

CFLAGS =  -ffreestanding -g
LDFLAGS = -nostdlib -T linker.ld -g

SRCS = boot.S kernel.c
OBJS = boot.o kernel.o

all: kernel8.img

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

kernel.o: kernel.c
	$(CC) $(CFLAGS) -c kernel.c -o kernel.o

kernel8.elf: $(OBJS) linker.ld
	$(LD) $(LDFLAGS) $(OBJS) -o kernel8.elf

kernel8.img: kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf kernel8.img

clean:
	rm -f *.o *.elf *.img

run: kernel8.img
	qemu-system-aarch64 -M virt -cpu cortex-a53 -nographic -kernel kernel8.img
