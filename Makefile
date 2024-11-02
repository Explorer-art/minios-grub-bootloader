ASM=nasm
SRC_DIR=src
BUILD_DIR?=build

all: clean minios.bin

minios.bin: $(BUILD_DIR)/minios.bin

$(BUILD_DIR)/minios.bin: $(SRC_DIR)/boot.asm always
	$(ASM) -f bin $< -o $@

always:
	mkdir build

clean:
	rm -rf build