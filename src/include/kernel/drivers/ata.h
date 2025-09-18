#ifndef ATA_H
#define ATA_H

#include <stdint.h>

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

uint8_t ata_initialize(uint16_t io_base, uint8_t drive);
uint8_t ata_identify(uint16_t io_base, uint8_t drive, uint8_t* buffer);
Drives detect_drives(void);
uint8_t ata_read_pio(uint16_t io_base, uint8_t drive, uint32_t lba, uint8_t sectors_count, uint8_t* dst);
uint8_t ata_write_pio(uint16_t io_base, uint8_t drive, uint32_t lba, uint32_t sectors, uint8_t* src);

#endif