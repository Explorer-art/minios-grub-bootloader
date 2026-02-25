global gdt_flush

gdt_flush:
	mov eax, [esp + 4]
	lgdt [eax]

	mov eax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x08:.flush

.flush:
	ret

global idt_flush

idt_flush:
	mov eax, [esp + 4]
	lidt [eax]
	ret

global inb

inb:
	mov edx, [esp + 4]
	in al, dx
	ret

global outb

outb:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, al
	ret

global inw

inw:
	mov edx, [esp + 4]
	in ax, dx
	ret

global outw

outw:
	mov edx, [esp + 4]
	mov eax, [esp + 8]
	out dx, ax
	ret

global paging_enable

paging_enable:
    mov eax, cr0
    or eax, 0x80000000
    mov cr0, eax
    ret

global paging_disable

paging_disable:
    mov eax, cr0
    and eax, 0x7FFFFFFF
    mov cr0, eax
    ret

global switch_context

switch_context:
	mov eax, [esp + 4]	; **old_context
	mov edx, [esp + 8]	; *new_context

	cmp eax, 0
	je .not_old_context

	push ebp
	push ebx
	push esi
	push edi

	mov [eax], esp
	
.not_old_context:
	mov esp, edx

	pop edi
	pop esi
	pop ebx
	pop ebp
	ret					; pop eip and jmp
