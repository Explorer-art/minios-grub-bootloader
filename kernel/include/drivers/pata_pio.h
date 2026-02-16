#ifndef ATA_H
#define ATA_H

#include <stdint.h>
#include <stdbool.h>

#define ATA_SECTOR_SIZE 512

#define ATA_PRIMARY_IO 0x1F0
#define ATA_SECONDARY_IO 0x170

#define ATA_PRIMARY_DEVCTL 0x3F6
#define ATA_SECONDARY_DEVCTL 0x376

typedef struct {
	uint8_t PrimaryMaster;
	uint8_t PrimarySlave;
	uint8_t SecondaryMaster;
	uint8_t SecondarySlave;
} Drives;

uint8_t pata_pio_init(uint16_t io_base, uint8_t drive);
uint8_t pate_pio_identify(uint16_t io_base, uint8_t drive, uint8_t* buffer);
void pata_pio_detect_drives(Drives* drives);
bool pata_pio_read_sectors(uint16_t io_base, uint8_t drive, uint32_t lba, uint8_t count, uint8_t* buffer);
bool pata_pio_write_sectors(uint16_t io_base, uint8_t drive, uint32_t lba, uint32_t count, uint8_t* buffer);

#endif