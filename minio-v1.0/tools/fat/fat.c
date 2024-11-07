#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

typedef uint8_t bool;

#define false 0
#define true 1

typedef struct {
	uint8_t BootJumpInstruction[3];
	uint8_t OemIdentifier[8];
	uint16_t BytesPerSector;
	uint8_t SectorsPerCluster;
	uint16_t ReservedSectors;
	uint8_t FatCount;
	uint16_t DirEntriesCount;
	uint16_t TotalSectors;
	uint8_t MediaDescriptorType;
	uint16_t SectorsPerFat;
	uint16_t SectorsPerTrack;
	uint32_t HiddenSectors;
	uint32_t LargeSectorCount;

	// extended boot sector

	uint8_t drive_number;
	uint8_t Reserved;
	uint8_t Signature;
	uint32_t VolumeId;
	uint8_t VolumeLabler[11];
	uint8_t SystemId[8];
} __attribute__((packed)) BootSector;

typedef struct {
	uint8_t Filename[11];
	uint8_t Attributes;
	uint8_t Reserved;
	uint8_t CreatedTimeTenths;
	uint16_t CreatedTime;
	uint16_t CreatedDate;
	uint16_t AccessedDate;
	uint16_t FirstClusterHigh;
	uint16_t ModificatedTime;
	uint16_t ModificatedDate;
	uint16_t FirstClusterLow;
	uint32_t Size;
} __attribute__((packed)) DirectoryEntry;

BootSector g_BootSector;
uint8_t* g_Fat = NULL;
DirectoryEntry* g_RootDirectory = NULL;
uint32_t g_RootDirectoryEnd;

bool ReadBootSector(FILE* disk) {
	return fread(&g_BootSector, sizeof(g_BootSector), 1, disk) > 0;
}

bool ReadSectors(FILE* disk, uint32_t lba, uint32_t count, void* output_buffer) {
	bool ok = true;

	ok = ok && (fseek(disk, lba * g_BootSector.BytesPerSector, SEEK_SET) == 0);
	ok = ok && (fread(output_buffer, g_BootSector.BytesPerSector, count, disk) == count);

	return ok;
}

bool ReadFat(FILE* disk) {
	g_Fat = (uint8_t*) malloc(g_BootSector.SectorsPerFat * g_BootSector.BytesPerSector);

	return ReadSectors(disk, g_BootSector.ReservedSectors, g_BootSector.SectorsPerFat, g_Fat);
}

bool ReadRootDirectory(FILE* disk) {
	uint32_t lba = g_BootSector.ReservedSectors + g_BootSector.SectorsPerFat * g_BootSector.FatCount;
	uint32_t size = sizeof(DirectoryEntry) * g_BootSector.DirEntriesCount;
	uint32_t sectors = size / g_BootSector.BytesPerSector;

	if (size % g_BootSector.BytesPerSector > 0) {
		sectors++;
	}

	g_RootDirectoryEnd = lba + sectors;
	g_RootDirectory = (DirectoryEntry*) malloc(sectors * g_BootSector.BytesPerSector);
	return ReadSectors(disk, lba, sectors, g_RootDirectory);
}

DirectoryEntry* FindFile(const char* filename) {
	for (uint32_t i = 0; i < g_BootSector.DirEntriesCount; i++) {
		if (memcmp(filename, g_RootDirectory[i].Filename, 11) == 0) {
			return &g_RootDirectory[i];
		}
	}

	return NULL;
}

bool ReadFile(FILE* disk, DirectoryEntry* file_entry, uint8_t* output_buffer) {
	bool ok = true;
	uint16_t current_cluster = file_entry->FirstClusterLow;

	do {
		uint32_t lba = g_RootDirectoryEnd + (current_cluster - 2) * g_BootSector.SectorsPerCluster;
		ok = ok && ReadSectors(disk, lba, g_BootSector.SectorsPerCluster, output_buffer);
		output_buffer += g_BootSector.SectorsPerCluster * g_BootSector.BytesPerSector;

		uint32_t fat_index = current_cluster * 3 / 2;

		if (current_cluster % 2 == 0)
			current_cluster = (*(uint16_t*)(g_Fat + fat_index)) & 0x0FFF;
		else
			current_cluster = (*(uint16_t*)(g_Fat + fat_index)) >> 4;

	} while (ok && current_cluster < 0x0FF8);

	return ok;
}

int main(int argc, char* argv[]) {
	if (argc < 3) {
		printf("Syntax: %s <disk image> <file name>\n", argv[0]);
		return -1;
	}

	FILE* disk = fopen(argv[1], "rb");

	if (disk == NULL) {
		printf("ERROR: Open file failed\n");
		return -1;
	}

	if (!ReadBootSector(disk)) {
		printf("ERROR: Read boot sector failed.\n");
		return -1;
	}

	if (!ReadFat(disk)) {
		printf("ERROR: Read FAT failed.\n");
		free(g_Fat);
		return -1;
	}

	if (!ReadRootDirectory(disk)) {
		printf("ERROR: Read root directory.\n");
		free(g_Fat);
		free(g_RootDirectory);
		return -1;
	}

	printf(g_RootDirectory[0].Filename);
	printf("\n");

	DirectoryEntry* file_entry = FindFile(argv[2]);

	if (file_entry == NULL) {
		printf("ERROR: File not found.\n");
		free(g_Fat);
		free(g_RootDirectory);
		return -1;
	}

	// uint8_t* buffer = (uint8_t*) malloc(file_entry->Size + g_BootSector.BytesPerSector);
	//
	// if (!ReadFile(disk, file_entry, buffer)) {
	// 	printf("ERROR: Read file failed.\n");
	// 	free(g_Fat);
	// 	free(g_RootDirectory);
	// 	free(buffer);
	// 	return -1;
	// }
	//
	// for (size_t i = 0; i < file_entry->Size; i++) {
	// 	fputc(buffer[i], stdout);
	// }
	//
	// free(g_Fat);
	// free(g_RootDirectory);
	// free(buffer);

	return 0;
}
