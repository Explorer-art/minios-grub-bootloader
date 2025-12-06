#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/cpu/pic.h>
#include <kernel/mm/vmm.h>
#include <kernel/drivers/tty.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/drivers/ata.h>
#include <kernel/utils/kprintf.h>

extern void sctest_write(void);
extern void sctest_read(void);

void kmain() {
	tty_init();
	gdt_init();
	idt_init();
    pic_remap(0x20, 0x28);
    vmm_init();

    Drives drives = detect_drives();

    keyboard_init();

    __asm__ volatile ("sti");

    uint32_t* ptr = (uint32_t*)0x400000;
    *ptr = 0x100;

	for(;;);
}
