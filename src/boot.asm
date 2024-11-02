bits 16
org 0x7C00

%define ENDL 0Ah, 0Dh

start:
	; инициализация сегментов
	
	cli
	xor ax, ax
	mov ds, ax
	mov es, ax
	mov ss, ax
	sti

	call clear

	mov si, msg_loading
	call puts

	; чтение операционной системы с диска
	
	mov ah, 02h
	mov al, 5 ; количество секторов
	mov ch, 0 ; цилиндр
	mov cl, 2 ; начальный сектор для чтения
	mov dh, 0 ; головка
	mov dl, 0x80 ; диск
	xor bx, bx
	mov si, bx
	mov bx, 0x7E00 ; адрес начала памяти куда будут загружены данные
	int 13h

	jc read_error

	jmp 0x7E00

read_error:
	mov si, msg_read_error
	call puts

	cli
	hlt

%include "src/io.asm"

msg_loading db "Loading...", ENDL, 0
msg_read_error db "ERROR: Read from disk failed!", ENDL, 0

times 510 - ($-$$) db 0
dw 0xAA55

%include "src/kernel.asm"

%include "src/terminal.asm"

%include "src/strings.asm"


