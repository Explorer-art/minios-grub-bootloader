#include <drivers/pata_pio.h>
#include <x86.h>
#include <utils/kprintf.h>

static void pata_pio_delay(uint8_t delay) {
	for (uint8_t i = 0; i < delay; i++) {
		inb(ATA_SECONDARY_DEVCTL);
	}
}

static void pata_pio_wait_bsy(uint16_t io_base) {
	while (inb(io_base + 7) & 0x80);
}

static void pata_pio_wait_drq(uint16_t io_base) {
	while (!(inb(io_base + 7) & 0x08));
}

uint8_t pata_pio_init(uint16_t io_base, uint8_t drive) {
	outb(io_base + 6, drive);
	outb(io_base + 2, 0);
	outb(io_base + 3, 0);
	outb(io_base + 4, 0);
	outb(io_base + 2, 0);
	outb(io_base + 7, 0xEC); // ATA IDENTIFY

	pata_pio_delay(4);
	uint8_t status = inb(io_base + 7);

	if (status == 0 || status == 0xFF) {
		kprintf("warning: No ATA device detected (channel=%x, drive=%x)\n", io_base, drive);
		return 1;
	}

	pata_pio_wait_bsy(io_base);

	// wait for error or drq
	while (1) {
		uint8_t status = inb(io_base + 7);
		if (status & 0x08) {
			for (uint8_t i = 0; i < 255; i++) {
				inb(io_base);
			}
			kprintf("info: ATA device initialize (channel=%x, drive=%x)\n", io_base, drive);
			return 0;
		}
		if (status & 0x01) {
			kprintf("error: ATA device error (channel=%x, drive=%x)\n", io_base, drive);
			return 2;
		}
	}
}

uint8_t pate_pio_identify(uint16_t io_base, uint8_t drive, uint8_t* buffer) {
	if (!buffer)
		return -1;

	outb(io_base + 6, drive);
	outb(io_base + 2, 0);
	outb(io_base + 3, 0);
	outb(io_base + 4, 0);
	outb(io_base + 2, 0);
	outb(io_base + 7, 0xEC); // ATA IDENTIFY

	pata_pio_delay(4);
	uint8_t status = inb(io_base + 7);

	if (status == 0 || status == 0xFF) {
		kprintf("warning: No ATA device detected (channel=%x, drive=%x)\n", io_base, drive);
		return 1;
	}

	pata_pio_wait_bsy(io_base);

	// wait for error or drq
	while (1) {
		uint8_t status = inb(io_base + 7);
		if (status & 0x08) {
			for (uint8_t i = 0; i < 255; i++) {
				buffer[i] = inb(io_base);
			}
			kprintf("info: ATA device identify (channel=%x, drive=%x)\n", io_base, drive);
			return 0;
		}
		if (status & 0x01) {
			kprintf("error: ATA device error (channel=%x, drive=%x)\n", io_base, drive);
			return 2;
		}
	}
}

void pata_pio_detect_drives(Drives* drives) {
	drives->PrimaryMaster = pata_pio_init(ATA_PRIMARY_IO, 0xE0);
	drives->PrimarySlave = pata_pio_init(ATA_PRIMARY_IO, 0xF0);
	drives->SecondaryMaster = pata_pio_init(ATA_SECONDARY_IO, 0xE0);
	drives->SecondarySlave = pata_pio_init(ATA_SECONDARY_IO, 0xF0);
}

bool pata_pio_read_sectors(uint16_t io_base, uint8_t drive, uint32_t lba, uint8_t count, uint8_t* buffer) {
	if (!buffer || !count)
		return false;

	outb(io_base + 6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
	outb(io_base + 2, count);
	outb(io_base + 3, (uint8_t)(lba));
	outb(io_base + 4, (uint8_t)(lba >> 8));
	outb(io_base + 5, (uint8_t)(lba >> 16));
	outb(io_base + 7, 0x20); // Send the read command

	for (int i = 0; i < count; i++) {
		pata_pio_delay(4);
		pata_pio_wait_bsy(io_base);
		pata_pio_wait_drq(io_base);

		for (uint16_t j = 0; j < ATA_SECTOR_SIZE / 2; j++) {
			((uint16_t*)buffer)[j] = inw(io_base);
		}

		buffer += ATA_SECTOR_SIZE;
	}

	return true;
}

bool pata_pio_write_sectors(uint16_t io_base, uint8_t drive, uint32_t lba, uint32_t count, uint8_t* buffer) {
	if (!buffer || !count)
		return false;

	outb(io_base + 6, 0xE0 | (drive << 4) | ((lba >> 24) & 0x0F));
	outb(io_base + 2, count);
	outb(io_base + 3, (uint8_t)lba);
	outb(io_base + 4, (uint8_t)(lba >> 8));
	outb(io_base + 5, (uint8_t)(lba >> 16));
	outb(io_base + 7, 0x30); // Send the write command

	for (uint16_t i = 0; i < count; i++) {
		pata_pio_delay(4);
		pata_pio_wait_bsy(io_base);
		pata_pio_wait_drq(io_base);

		for (uint16_t j = 0; j < ATA_SECTOR_SIZE / 2; j++) {
			uint16_t word = buffer[j] | (buffer[j + 1] << 8);
			outw(io_base, word);
		}

		buffer += ATA_SECTOR_SIZE;
	}

	pata_pio_wait_bsy(io_base);
	outb(io_base + 7, 0xE7);
	pata_pio_wait_bsy(io_base);

	return true;
}
