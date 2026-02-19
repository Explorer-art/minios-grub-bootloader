LD = ld
CFLAGS= -m32 -ffreestanding -fno-stack-protector -fno-builtin -Wall -Wextra -I src/include 
SRC_DIR ?= src
BUILD_DIR ?= build
KERNEL_BUILD_DIR ?= kernel/build
LOOP := $(shell losetup -f)

PHONY: all clean always

all: clean always minios.img

minios.img:
	make -C kernel
	make -C userland/phasma

	mkdir mnt
	
	# Create image
	dd if=/dev/zero of=minios.img bs=1M count=64
	
	parted minios.img mklabel msdos
	parted -a minimal minios.img mkpart primary fat32 1MiB 100%

	# Mount
	sudo losetup -fP minios.img
	sudo mkfs.fat -F 32 -n MINIOS $(LOOP)p1
	sudo mount -t vfat $(LOOP)p1 mnt

	# Copy files
	sudo mkdir -p mnt/boot/grub
	sudo mkdir -p mnt/etc
	sudo cp $(KERNEL_BUILD_DIR)/minios.bin mnt/boot/
	sudo cp rootfs/boot/grub/grub.cfg mnt/boot/grub/
	sudo cp rootfs/etc/autoexec.cfg mnt/etc/
	sudo cp userland/phasma/phasma.bin mnt/

	# GRUB install
	sudo grub-install --target=i386-pc --boot-directory=mnt/boot --recheck $(LOOP)

	# Unmount
	sudo umount mnt
	sudo losetup -d $(LOOP)

	sudo chmod 777 minios.img

always:
	mkdir -p $(BUILD_DIR)

clean:
	rm -f -r minios.img
	rm -f -r $(BUILD_DIR)
	rm -f -r mnt

run:
	qemu-system-x86_64 -drive format=raw,file=minios.img,if=ide,index=0