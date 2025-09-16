global sctest_write
global sctest_read

sctest_write:
	mov eax, 1
	mov ebx, 1
	mov edx, 0
	mov ecx, 'X'
	int 0x80
	ret

sctest_read:
	mov eax, 0
	mov ebx, 0
	int 0x80
	ret