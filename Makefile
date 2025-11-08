# Makefile for x86 Kernel
#
# This Makefile compiles the kernel and creates a bootable ISO image

# Compiler and tools
CC = gcc
LD = ld
QEMU = qemu-system-i386

# Compiler flags
# -m32: Compile for 32-bit x86
# -std=c11: Use C11 standard
# -ffreestanding: Don't assume standard library exists
# -nostdlib: Don't link standard library
# -nostdinc: Don't include standard headers
# -fno-builtin: Don't use built-in functions
# -Wall: Enable all warnings
# -Wextra: Enable extra warnings
# -g: Include debug symbols (for GDB)
# -O2: Optimize for speed
CFLAGS = -m32 -std=c11 -ffreestanding -nostdlib -nostdinc -fno-builtin -Wall -Wextra -g -O2

# Linker flags
# -m elf_i386: Output 32-bit ELF format
# -T linker.ld: Use our custom linker script
# -g: Include debug symbols
LDFLAGS = -m elf_i386 -T linker.ld -g

# Directories
BUILD_DIR = build
ISO_DIR = iso
BOOT_DIR = $(ISO_DIR)/boot
GRUB_DIR = $(BOOT_DIR)/grub

# Source files
KERNEL_SRC = boostrap.c vga.c serial.c debug.c idt.c idt_asm.c pic.c
KERNEL_OBJ = $(BUILD_DIR)/boostrap.o $(BUILD_DIR)/vga.o $(BUILD_DIR)/serial.o $(BUILD_DIR)/debug.o $(BUILD_DIR)/idt.o $(BUILD_DIR)/idt_asm.o $(BUILD_DIR)/pic.o
KERNEL_BIN = $(BUILD_DIR)/kernel.bin

# Default target
all: $(KERNEL_BIN) iso

# Create build directory
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile kernel source files to object files
$(BUILD_DIR)/boostrap.o: boostrap.c debug.h idt.h pic.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/vga.o: vga.c vga.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/serial.o: serial.c serial.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/debug.o: debug.c debug.h vga.h serial.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt.o: idt.c idt.h debug.h pic.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/idt_asm.o: idt_asm.c idt.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/pic.o: pic.c pic.h | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link object file to create kernel binary
$(KERNEL_BIN): $(KERNEL_OBJ) linker.ld
	$(LD) $(LDFLAGS) $(KERNEL_OBJ) -o $@

# Create bootable ISO
iso: $(KERNEL_BIN) grub.cfg
	mkdir -p $(GRUB_DIR)
	cp $(KERNEL_BIN) $(BOOT_DIR)/
	cp grub.cfg $(GRUB_DIR)/
	grub-mkrescue -o kernel.iso $(ISO_DIR)

# Run kernel in QEMU
# -serial stdio: Redirect serial port (COM1) to stdout
# -monitor stdio: Enable QEMU monitor (press Ctrl+A then C to access)
run: iso
	$(QEMU) -cdrom kernel.iso -serial stdio

# Run kernel in QEMU with serial output to file
# -serial file:serial.log: Redirect serial port to file
run-log: iso
	$(QEMU) -cdrom kernel.iso -serial file:serial.log -monitor stdio

# Run kernel in QEMU with GDB server
# -s: Shorthand for -gdb tcp::1234 (start GDB server on port 1234)
# -S: Freeze CPU at startup (wait for GDB to connect)
debug: iso
	@echo "Starting QEMU with GDB server on port 1234..."
	@echo "In another terminal, run: gdb -ex 'target remote localhost:1234' -ex 'symbol-file build/kernel.bin'"
	$(QEMU) -cdrom kernel.iso -serial stdio -s -S

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) kernel.iso serial.log

# Phony targets (not actual files)
.PHONY: all iso run run-log debug clean

