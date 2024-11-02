;
; Основной цикл терминала
;

terminal_loop:
	mov si, command_prefix
	call puts

	call gets

	mov bx, buffer
	call command_handler

	jmp terminal_loop

;
; Обработчик команд
;
; Параметры:
; - bx: адрес начала буфера
;

command_handler:
	lea si, [bx] ; загружаем в регистр SI адрес начала буффера

	push bx
	lea bx, [command_help] ; загружаем в регистр BX адрес начала команды
	call compare_string ; вызываем функцию сравнения строк
	pop bx

	cmp cx, 0 ; проверяем результат (если CX = 0 - строки равны, если CX = 1 - строки не равны)
	je command_help_executor ; если команда найдена выполянем функцию команды

	push bx
	lea bx, [command_info]
	call compare_string
	pop bx
	
	cmp cx, 0
	je command_info_executor

	jmp unknown_command ; если команда не найдена выводим соотвутствующие сообщение на экран

command_help_executor:
	mov si, msg_help
	call puts
	ret

command_info_executor:
	mov si, msg_info
	call puts
	ret

unknown_command:
	mov si, msg_unknown_command
	call puts
	ret

;
; Получить n-ый аргумент команды
;
; Параметры:
; - ax: номер аргумента
;
; Выход:
; - bx: указатель на начало аргумента
;

get_arg:
	push dx
	push cx

	xor cx, cx	
	lea si, [buffer]
	
.loop:
	cmp ax, cx
	je .get_arg
	
	cmp byte [si], 20h
	je .add_arg

	inc si

	jmp .loop

.add_arg:
	inc cx
	inc si
	jmp .loop

.get_arg:
	lea di, [arg_buffer]
	
.get_arg_loop:
	cmp byte [si], 20h
	je .done

	cmp byte [si], 0
	je .done

	mov dx, [si]
	mov [di], dx
	inc si
	inc di

	jmp .get_arg_loop

.done:
	mov bx, arg_buffer

	pop cx
	pop dx
	ret

command_prefix db "> ", 0
command_help db "help", 0
command_info db "info", 0
command_echo db "echo", 0
msg_help db "COMMANDS:", ENDL, "help - help by commands", ENDL, "info - get information about the system", ENDL, "echo - print a line to the screen", ENDL, 0
msg_info db "System: Minios", ENDL, "Version: v0.1", ENDL, "Author: Truzme_", ENDL, 0
msg_unknown_command db "Unknown command", ENDL, 0

buffer times 1024 db 0

arg_buffer times 1024 db 0
