bits 32
org 0

section .text

start:
	mov esp, stack_space
	hlt

section .bss
resb 8192
stack_space:
