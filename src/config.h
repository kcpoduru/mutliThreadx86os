#ifndef CONFIG_H
#define CONFIG_H

#define KERNEL_CODE_SELECTOR                0x08
#define PKOS_TOTAL_INTERRUPTS				512

#define PKOS_HEAP_SIZE_BYTES                104857600
#define PKOS_HEAP_BLOCK_SIZE                4096
#define PKOS_HEAP_ADDRESS                   0x01000000 
#define PKOS_HEAP_TABLE_ADDRESS             0x00007E00
#define PKOS_SECTOR_SIZE                    512

#define PKOS_MAX_FILESYSTEMS                12
#define PKOS_MAX_FILE_DESCRIPTORS           512

#endif
