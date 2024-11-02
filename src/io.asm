;
; Очистка экрана
;

clear:
	push ax
	
	mov ah, 00h
	mov al, 02h
	int 10h

	pop ax
	
	ret

;
; Новая строка
;
new_line:
	push ax

	mov ah, 0Eh
	mov al, 0Ah
	int 10h
	mov al, 0Dh
	int 10h

	pop ax
	ret

;
; Вывод символа на экран
;
; Параметры:
; - al,
;

putchar:
	push ax

	mov ah, 0Eh
	int 10h

	pop ax
	ret

;
; Вывод строки на экран
;
; Параметры:
; - si: указатель на строку
;

puts:
	push ax

	mov ah, 0Eh
.loop:
	lodsb
	cmp al, 0
	je .done
	int 10h


	jmp .loop
.done:
	pop ax
	ret

;
; Функция получения введенного символа
;
; Параметры:
; - отсутствуют
;
; Выход:
; - ah: скан-код клавиши
; - al: ASCII код символа

getchar:
	mov ah, 00h
	int 16h
	
	ret

;
; Функция ввода строки
;
; Параметры:
; - отсутствуют
;
; Выход:
; - строка в буффере
;

gets:
	call clear_buffer ; очищаем буффер

	lea si, [buffer]
	xor cx, cx

.loop:
	call getchar
	cmp ah, 1Ch
	je .done

	cmp ah, 0Eh
	je .input_backspace

	mov ah, 0Eh
	int 10h

	cmp cx, 1024
	jne .write_buffer
	
	jmp .loop

.write_buffer:
	mov [si], al
	inc si
	inc cx

	jmp .loop

.input_backspace:
	cmp cx, 0 ; проверка номера символа по счету. Если это первый символ - возвращаемся обратно к функции ввода
	je .loop

	mov ah, 0Eh
	mov al, 08h
	int 10h
	mov al, 20h
	int 10h
	mov al, 08h
	int 10h

	dec cx
	mov byte [si - 1], 0

	jmp .loop

.done:
	call new_line
	ret

;
; Очистка буффера
;

clear_buffer:
	push cx

	xor cx, cx
	mov cx, 1024 ; размер буффера
	lea si, [buffer]

clear_loop:
	mov byte [si], 0
	inc si

	loop clear_loop

	pop cx
	ret
