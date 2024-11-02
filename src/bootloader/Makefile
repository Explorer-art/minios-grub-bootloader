ASM?=nasm
BUILD_DIR?=build

.PHONY: all clean

all: $(BUILD_DIR)/bootloader.bin

$(BUILD_DIR)/bootloader.bin:
	$(ASM) -f bin boot.asm -o $(BUILD_DIR)/bootloader.bin

clean:
	rm -f $(BUILD_DIR)/bootloader.bin