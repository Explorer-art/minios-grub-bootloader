ASM=nasm
CC=gcc
LD=ld
CFLAGS=-m32 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I src/include
SRC_DIR?=src
BUILD_DIR?=build

OBJECTS=\
$(BUILD_DIR)/boot/boot.o \
$(BUILD_DIR)/kernel/drivers/gdt.o \
$(BUILD_DIR)/kernel/kernel.o \
$(BUILD_DIR)/kernel/gdt.o \
$(BUILD_DIR)/kernel/tty.o \
$(BUILD_DIR)/libc/string.o \
$(BUILD_DIR)/libc/memory.o \

PHONY: all clean always

all: clean always minio.iso

minio.iso: $(BUILD_DIR)/minio.iso

$(BUILD_DIR)/minio.iso: minio.bin
	mkdir -p isodir/boot
	mkdir -p isodir/boot/grub
	cp $(BUILD_DIR)/minio.bin isodir/boot/minio.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o $(BUILD_DIR)/minio.iso isodir

minio.bin: $(BUILD_DIR)/minio.bin

$(BUILD_DIR)/minio.bin: $(OBJECTS)
	$(LD) -m elf_i386 -T $(SRC_DIR)/linker.ld -o $@ $(OBJECTS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.s
	$(ASM) -f elf32 $< -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

always:
	mkdir -p $(BUILD_DIR)/boot
	mkdir -p $(BUILD_DIR)/kernel
	mkdir -p $(BUILD_DIR)/kernel/drivers
	mkdir -p $(BUILD_DIR)/libc

clean:
	rm -f -R $(BUILD_DIR)/*
	rm -f -R isodir