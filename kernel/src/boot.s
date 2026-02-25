bits 32

section .text
	; multiboot spec
	align 4
	dd 0x1BADB002					; magic
	dd 0x00000000					; flags
	dd -(0x1BADB002 + 0x00000000)	; checksum

global start
extern kmain

start:
	cli
	mov esp, stack_space
	push ebx
	push eax
	call kmain
	hlt

section .bss
resb 4096
stack_space:
