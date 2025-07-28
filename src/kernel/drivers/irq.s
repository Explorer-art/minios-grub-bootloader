global irq1

extern keyboard_handler

irq1:
	push ds
	push es
	push fs
	push gs
	pushad

	call keyboard_handler

	popad
	pop gs
	pop fs
	pop es
	pop ds

	iretd