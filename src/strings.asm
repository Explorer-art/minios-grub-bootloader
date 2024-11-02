;
; Сравнение двух строк
;
; Параметры:
; - si: строка 1
; - bx: строка 2
;
; Выход:
; - cx: 1 - строки равны, 0 - строки не равны
;

compare_string:
	push ax

	; проверка длин строк
	push bx
	mov bx, si
	call strlen
	mov dx, cx
	pop bx
	call strlen
	
	cmp cx, dx
	jne .not_equal

.loop:
	cmp byte [si], 0
	je .equal
	
	mov ah, [bx]
	cmp [si], ah
	jne .not_equal
	inc si
	inc bx

	jmp .loop

.equal:
	mov cx, 0
	jmp .done

.not_equal:
	mov cx, 1
	jmp .done

.done:
	pop ax
	ret

;
; Получить длину строки
;
; Параметры:
; - bx: указатель на начало строки
;
; Выход:
; - cx: длина строки
;

strlen:
	push bx
	push si

	xor cx, cx
	lea si, [bx]

.loop:
	cmp byte [si], 0
	je .done

	inc cx
	inc si

	jmp .loop

.done:
	pop si
	pop bx
	ret
