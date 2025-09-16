#include <kernel/cpu/gdt.h>
#include <kernel/cpu/idt.h>
#include <kernel/pic.h>
#include <kernel/drivers/tty.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/console.h>

extern void sctest_write(void);
extern void sctest_read(void);

void kmain() {
	tty_init();
	gdt_init();
	idt_init();
    pic_remap(0x20, 0x28);

    keyboard_init();

    __asm__ volatile ("sti");

    sctest_read();

	for(;;);
}
