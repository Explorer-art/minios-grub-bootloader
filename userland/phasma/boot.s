bits 32
org 0x80000000

section .text

start:
	mov eax, 1
	mov ebx, msg
	mov ecx, 1
	int 0x80

	mov eax, 2
	int 0x80

msg	db "Hello from userspace!", 0x0A, 0
