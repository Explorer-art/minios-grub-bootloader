#include <kernel/disk.h>
#include <kernel/x86_disk.h>

/*
	Функция конвентации LBA адреса в CHS адрес
*/

void DISK_LBA2CHS(DISK* disk, uint32_t lba, uint16_t* cylinder_out, uint16_t* sector_out, uint16_t* head_out) {
	*cylinder_out = (lba / disk->sectors) / disk->heads;
	*sector_out = lba % disk->sectors + 1;
	*head_out = (lba / disk->sectors) % disk->heads;
}

/*
	Функция инициализации диска
*/

bool DISK_Initialize(DISK* disk, uint8_t drive_number) {
	uint8_t drive_type;
	uint16_t cylinders, sectors, heads;

	if (!disk_get_params(disk->id, &drive_type, &cylinders, &sectors, &heads)) {
		return false;
	}

	disk->id = drive_number;
	disk->cylinders = cylinders;
	disk->sectors = sectors;
	disk->heads = heads;

	return true;
}

/*
	Функция чтения секторов с диска
*/

bool DISK_ReadSectors(DISK* disk, uint32_t lba, uint8_t sectors, void far* data_out) {
	uint16_t cylinder, sector, head;

	DISK_LBA2CHS(disk, lba, &cylinder, &sector, &head);

	for (int i = 0; i < 3; i++) {
		if (disk_read(disk->id, cylinder, sector, head, sectors, data_out)) {
			return true;
		}

		disk_reset(disk->id);
	}

	return false;
}
