# Toolchain
CROSS_COMPILE = aarch64-linux-gnu-

CC := $(CROSS_COMPILE)gcc
AS := $(CROSS_COMPILE)as
LD := $(CROSS_COMPILE)ld

# Directories
SRC_DIR := src
BUILD_DIR := build
TARGET := $(BUILD_DIR)/kernel.elf

# File discovery - find all .c and .S files in src
S_SOURCES := $(shell find $(SRC_DIR) -name *.S)
C_SOURCES := $(shell find $(SRC_DIR) -name *.c)

# Object File Mapping
# src/boot.S           -> build/boot.o
# src/lib/mmio/mmio.c  -> build/lib/mmio/mmio.o
S_OBJECTS := $(patsubst $(SRC_DIR)/%.S, $(BUILD_DIR)/%.o, $(S_SOURCES))
C_OBJECTS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SOURCES))
OBJECTS := $(S_OBJECTS) $(C_OBJECTS)

# Flags
# CFLAGS := -ffreestanding -g -Wall -Wextra -I$(SRC_DIR)
CFLAGS := -ffreestanding -g -Wall -Wextra
ASFLAGS := -g
LDFLAGS := -nostdlib -T linker.ld

# QEMU Config
# QEMU_FLAGS_RUN := -machine virt,virtualization=on,secure=on -nographic -cpu cortex-a72 -kernel $(TARGET)
# QEMU_FLAGS_RUN := -machine virt,virtualization=on -nographic -cpu cortex-a72 -kernel $(TARGET)
# QEMU_FLAGS_RUN := -machine virt -nographic -cpu cortex-a72 -kernel $(TARGET)
# QEMU_FLAGS_RUN := -machine virt -nographic -cpu cortex-a72 -kernel $(TARGET) -d in_asm
QEMU_FLAGS_RUN := -machine virt,gic-version=3 -m 8G -nographic -cpu cortex-a72 -kernel $(TARGET)
QEMU_FLAGS_DEBUG := -machine virt,gic-version=3 -m 8G -nographic -cpu cortex-a72 -kernel $(TARGET) -s -S

all: $(TARGET)

# Depends on all .o files
$(TARGET): $(OBJECTS)
	@echo "LD  $@"
	@mkdir -p $(dir $@)
	@$(LD) $(LDFLAGS) -o $@ $^

# Compile all .c files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC  $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# Compile all .S files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.S
	@echo "AS  $<"
	@mkdir -p $(dir $@)
	@$(AS) $(ASFLAGS) $< -o $@

# Run QEMU
run: all
	@qemu-system-aarch64 $(QEMU_FLAGS_RUN)

debug: all
	@qemu-system-aarch64 $(QEMU_FLAGS_DEBUG)

gdb:
	@gdb-multiarch build/kernel.elf

clean:
	@echo "Cleaning up..."
	@rm -rf $(BUILD_DIR)

dump_dtb:
	@qemu-system-aarch64 $(QEMU_FLAGS_RUN) -machine dumpdtb=qemu-virt.dtb
	@dtc -I dtb -O dts -o qemu-virt.dts qemu-virt.dtb
	@rm qemu-virt.dtb

