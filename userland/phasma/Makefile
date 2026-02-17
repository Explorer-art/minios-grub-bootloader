ASM = nasm
CC = gcc
LD = ld
CFLAGS = -m32 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I include -I ../libc/include
LIBC_DIR ?= ../../libc

OBJECTS = \
boot.o \
main.o \

LIBC_OBJECTS = \
$(LIBC_DIR)/build/ctype.o \
$(LIBC_DIR)/build/string.o \
$(LIBC_DIR)/build/memory.o \

PHONY: all clean

all: clean phasma.bin

phasma.bin: $(OBJECTS)
	make -C $(LIBC_DIR)

	$(LD) -m elf_i386 -T linker.ld -o $@ $(OBJECTS) $(LIBC_OBJECTS)

%.o: %.s
	$(ASM) -f elf32 $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o
	rm -f *.bin