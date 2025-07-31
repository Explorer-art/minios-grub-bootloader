#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>

#define KEYBOARD_BUFFER_SIZE	1024

#define KEY_ESC					1
#define KEY_LSHIFT				42
#define KEY_RSHIFT				54
#define KEY_ENTER           	28
#define KEY_BACKSPACE       	14
#define KEY_TAB             	15
#define KEY_CAPSLOCK			58
#define KEY_CTRL            	29
#define KEY_MOD             	91
#define KEY_ALT             	56
#define KEY_HOME            	71
#define KEY_INS             	82
#define KEY_DEL             	83
#define KEY_PGUP            	73
#define KEY_PGDOWN          	81
#define KEY_UP              	72
#define KEY_DOWN            	80
#define KEY_LEFT            	75
#define KEY_RIGHT           	77

void keyboard_init(void);
uint8_t keyboard_getchar(void);

#endif