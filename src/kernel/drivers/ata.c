#include <kernel/drivers/ata.h>
#include <kernel/cpu/port.h>
#include <kernel/console.h>

static void ata_delay(uint8_t delay) {
	for (uint8_t i = 0; i < delay; i++) {
		inb(ATA_SECONDARY_DEVCTL);
	}
}

static void ata_wait_bsy(void) {
	while (inb(ATA_PRIMARY_IO + 7) & 0x80);
}

static void ata_wait_drq(void) {
	while (!(inb(ATA_PRIMARY_IO + 7) & 0x08));
}

uint8_t ata_initialize(uint16_t io_base, uint8_t drive) {
	outb(io_base + 6, drive);
	outb(io_base + 2, 0);
	outb(io_base + 3, 0);
	outb(io_base + 4, 0);
	outb(io_base + 2, 0);
	outb(io_base + 7, 0xEC); // ATA IDENTIFY

	ata_delay(4);
	uint8_t status = inb(io_base + 7);

	if (status == 0 || status == 0xFF) {
		kprintf("warning: No ATA device detected\n");
		return 1;
	}

	ata_wait_bsy();

	// wait for error or drq
	while (1) {
		uint8_t status = inb(io_base + 7);
		if (status & 0x08) {
			for (uint8_t i = 0; i < 255; i++) {
				inb(io_base);
			}
			return 0;
		}
		if (status & 0x01) {
			return 2;
		}
	}
}

uint8_t ata_identify(uint16_t io_base, uint8_t drive, uint8_t* buffer) {
	if (!buffer)
		return -1;

	outb(io_base + 6, drive);
	outb(io_base + 2, 0);
	outb(io_base + 3, 0);
	outb(io_base + 4, 0);
	outb(io_base + 2, 0);
	outb(io_base + 7, 0xEC); // ATA IDENTIFY

	ata_delay(4);
	uint8_t status = inb(io_base + 7);

	if (status == 0 || status == 0xFF) {
		kprintf("warning: No ATA device detected\n");
		return 1;
	}

	ata_wait_bsy();

	// wait for error or drq
	while (1) {
		uint8_t status = inb(io_base + 7);
		if (status & 0x08) {
			for (uint8_t i = 0; i < 255; i++) {
				buffer[i] = inb(io_base);
			}
			return 0;
		}
		if (status & 0x01) {
			return 2;
		}
	}
}

Drives detect_drives(void) {
	Drives drives = {0};
	drives.PrimaryMaster = ata_initialize(ATA_PRIMARY_IO, 0xE0);
	drives.PrimaryMaster = ata_initialize(ATA_PRIMARY_IO, 0xF0);
	drives.PrimaryMaster = ata_initialize(ATA_SECONDARY_IO, 0xE0);
	drives.PrimaryMaster = ata_initialize(ATA_SECONDARY_IO, 0xF0);
	return drives;
}

uint8_t ata_read_pio(uint16_t io_base, uint8_t drive, uint32_t lba, uint8_t sectors, uint8_t* dst) {
	if (!dst || !sectors)
		return 1;

	outb(io_base + 6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
	outb(io_base + 2, sectors);
	outb(io_base + 3, (uint8_t)(lba));
	outb(io_base + 4, (uint8_t)(lba >> 8));
	outb(io_base + 5, (uint8_t)(lba >> 16));
	outb(io_base + 7, 0x20); // Send the read command

	for (int i = 0; i < sectors; i++) {
		ata_delay(4);
		ata_wait_bsy();
		ata_wait_drq();

		for (uint16_t j = 0; j < ATA_SECTOR_SIZE / 2; j++) {
			((uint16_t*)dst)[j] = inw(io_base);
		}

		dst += ATA_SECTOR_SIZE;
	}

	return 0;
}

uint8_t ata_write_pio(uint16_t io_base, uint8_t drive, uint32_t lba, uint32_t sectors, uint8_t* src) {
	if (!src || !sectors)
		return 1;

	outb(io_base + 6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
	outb(io_base + 2, sectors);
	outb(io_base + 3, (uint8_t)lba);
	outb(io_base + 4, (uint8_t)(lba >> 8));
	outb(io_base + 5, (uint8_t)(lba >> 16));
	outb(io_base + 7, 0x30); // Send the write command

	for (uint16_t i = 0; i < sectors; i++) {
		ata_delay(4);
		ata_wait_bsy();
		ata_wait_drq();

		for (uint16_t j = 0; j < ATA_SECTOR_SIZE / 2; j++) {
			uint16_t word = src[j] | (src[j + 1] << 8);
			outw(io_base, word);
		}

		src += ATA_SECTOR_SIZE;
	}

	ata_wait_bsy();
	outb(io_base + 7, 0xE7);
	ata_wait_bsy();

	return 0;
}