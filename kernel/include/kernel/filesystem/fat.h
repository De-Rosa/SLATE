#ifndef KERNEL_FILESYSTEM_FAT_H
#define KERNEL_FILESYSTEM_FAT_H

#include <stdint.h>

// https://wiki.osdev.org/FAT#BPB_(BIOS_Parameter_Block)
struct bpb {
	uint8_t jump[3];
	uint64_t oem_identifier;
	uint16_t bytes_per_sector;
	uint8_t sectors_per_cluster;
	uint16_t reserved_sectors_number;
	uint8_t fat_number;
	uint16_t root_directory_number;
	uint16_t total_sectors;
	uint8_t media_descriptor_type;
	uint16_t sectors_per_fat;
	uint16_t sectors_per_track;
	uint16_t heads_number;
	uint32_t hidden_sectors_number;
	uint32_t large_sector_count;
}__attribute__((packed));

// https://wiki.osdev.org/FAT#FAT_32_2
struct fat_32 {
	uint32_t size;
	uint16_t flags;
	uint16_t version;
	uint32_t cluster_number;
	uint16_t fsinfo_number;
	uint16_t backup_boot_number;
	uint8_t reserved0[8];
	uint8_t drive_number;
	uint8_t reserved1;
	uint8_t signature;
	uint32_t volume_id;
	uint8_t label[11];
	uint8_t system_identifier;
}__attribute__((packed));

#endif
