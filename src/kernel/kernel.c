#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/drivers/keyboard.h>
#include <kernel/console.h>

void kmain() {
	tty_init();
	gdt_init();
	idt_init();
    pic_remap();

    keyboard_init();

    __asm__ volatile ("sti");

    kprintf("Hello world from kernel!\n");

	for(;;);
}