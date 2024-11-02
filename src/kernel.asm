main:
	call clear

	mov si, msg_welcome
	call puts

	call new_line

	call terminal_loop

	cli
	hlt

msg_welcome db "Welcome to Minios!", ENDL, 0
