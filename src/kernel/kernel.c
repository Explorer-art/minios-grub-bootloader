#include <kernel/gdt.h>
#include <kernel/idt.h>
#include <kernel/pic.h>
#include <kernel/console.h>

void kmain() {
	tty_init();
	gdt_init();
	idt_init();
    pic_remap();

    kprintf("Hello world from kernel!");

	for(;;);
}