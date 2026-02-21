#include <cpu/gdt.h>
#include <cpu/idt.h>
#include <cpu/pic.h>
#include <mm/pmm.h>
#include <mm/vmm.h>
#include <drivers/pata_pio.h>
#include <drivers/tty.h>
#include <drivers/keyboard.h>
#include <drivers/timer.h>
#include <fs/fat32.h>
#include <process.h>
#include <utils/kprintf.h>
#include <utils/kmalloc.h>
#include <utils/config.h>

extern void sctest_write(void);
extern void sctest_read(void);

void kmain(uint32_t magic) {
	tty_init();
	gdt_init();
	idt_init();
    pic_remap(0x20, 0x28);

    pmm_init();
    vmm_init();

    Drives drives = {0};
    pata_pio_detect_drives(&drives);

    fat32_ctx_t fat32_ctx = {0};

    if (!fat32_init(&fat32_ctx, 2048)) {
        kprintf("error: FAT32 init failed\n");
        goto pause;
    }

    kprintf("Bytes per sector: %d\n", fat32_ctx.bytes_per_sector);

    keyboard_init();
    timer_init(20);

    __asm__ volatile ("sti");

    fat32_file_t file;
    char buffer[512];
    int bytes_read = 0;

    if (!fat32_open_file(&fat32_ctx, &file, "/program.bin")) {
        kpanic("Cannot open program.bin");
    }

    bytes_read = fat32_read(&file, buffer, sizeof(buffer));
    fat32_close(&file);

    process_create(buffer, bytes_read);

pause:
	for(;;);
}
