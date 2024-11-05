bits 16
org 0x7C00

%define ENDL 0Ah, 0Dh

jmp short start
nop



;
; FAT12 HEADER
;

bdb_oem:                            db "MSWIN4.1"           ; 8 bytes
bdb_bytes_per_sector:               dw 512
bdb_sectors_per_cluster:            db 1
bdb_reserved_sectors:               dw 1
bdb_fat_count:                      db 2
bdb_dir_entries_count:              dw 0E0h
bdb_total_sectors:                  dw 2880                 ; 2880 * 512 = 1.44 MB
bdb_media_descriptor_type:          db 0F0h                 ; F0 = 3.5" floppy disk
bdb_sectors_per_fat:                dw 9                    ; 9 sectors/fat
bdb_sectors_per_track:              dw 18
bdb_heads:                          dw 2
bdb_hidden_sectors:                 dd 0
bdb_large_sector_count:             dd 0

; extended boot record

ebr_drive_number:                   db 0
                                    db 0
ebr_signature:                      db 29h
ebr_volume_id:                      db 10h, 15h, 17h, 18h
ebr_volume_label:                   db "MINIOS     "        ; 11 bytes
ebr_system_id:                      db "FAT12   "           ; 8 bytes

;
; CODE
;

start:
    ; setup data segments
    xor ax, ax
    mov ds, ax
    mov es, ax

    ; setup stack
    mov ss, ax
    mov sp, 0x7C00

    push es
    push word .after
    retf

.after:

    ; read something from floppy disk
    ; BIOS should set DL to drive number
    mov [ebr_drive_number], dl

    ; show loading message
    mov si, msg_loading
    call puts

    ; read drive parameters (sectors per track and head count),
    ; instead of relying on data on formatted disk
    push es
    mov ah, 08h
    int 13h
    jc floppy_error
    pop es

    and cl, 0x3F                        ; remove top 2 bits
    xor ch, ch
    mov [bdb_sectors_per_track], cx     ; sector count

    inc dh
    mov [bdb_heads], dh                 ; head count

    ; compute LBA of root directory = reserved + fats * sectors_per_fat
    ; note: this section can be hardcoded
    mov ax, [bdb_sectors_per_fat]
    mov bl, [bdb_fat_count]
    xor bh, bh
    mul bx                              ; ax = (fats * sectors_per_fat)
    add ax, [bdb_reserved_sectors]      ; ax = LBA of root directory
    push ax

    ; compute size of root directory = (32 * number_of_entries) / bytes_per_sector
    mov ax, [bdb_dir_entries_count]
    shl ax, 5                           ; ax *= 32
    xor dx, dx                          ; dx = 0
    div word [bdb_bytes_per_sector]     ; number of sectors we need to read

    test dx, dx                         ; if dx != 0, add 1
    jz .root_dir_after
    inc ax                              ; division remainder != 0, add 1
                                        ; this means we have a sector only partially filled with entries
.root_dir_after:

    ; read root directory
    mov cl, al                          ; cl = number of sectors to read = size of root directory
    pop ax                              ; ax = LBA of root directory
    mov dl, [ebr_drive_number]          ; dl = drive number (we saved it previously)
    mov bx, buffer                      ; es:bx = buffer
    call disk_read

    ; search for kernel.bin
    xor bx, bx
    mov di, buffer

.search_kernel:
    mov si, file_kernel_bin
    mov cx, 11                          ; compare up to 11 characters
    push di
    repe cmpsb
    pop di
    je .found_kernel

    add di, 32
    inc bx
    cmp bx, [bdb_dir_entries_count]
    jl .search_kernel

    ; kernel not found
    jmp kernel_not_found_error

.found_kernel:
    ; di should have the address to the entry
    mov ax, [di + 26]                   ; first logical cluster field (offset 26)
    mov [kernel_cluster], ax

    ; load FAT from disk into memory
    mov ax, [bdb_reserved_sectors]
    mov bx, buffer
    mov cl, [bdb_sectors_per_fat]
    mov dl, [ebr_drive_number]
    call disk_read

    ; read kernel and process FAT chain
    mov bx, KERNEL_LOAD_SEGMENT
    mov es, bx
    mov bx, KERNEL_LOAD_OFFSET

.load_kernel_loop:
    ; Read next cluster
    mov ax, [kernel_cluster]
    
    ; not nice :( hardcoded value
    add ax, 31                          ; first cluster = (kernel_cluster - 2) * sectors_per_cluster + start_sector
                                        ; start sector = reserved + fats + root directory size = 1 + 18 + 134 = 33
    mov cl, 1
    mov dl, [ebr_drive_number]
    call disk_read

    add bx, [bdb_bytes_per_sector]

    ; compute location of next cluster
    mov ax, [kernel_cluster]
    mov cx, 3
    mul cx
    mov cx, 2
    div cx                              ; ax = index of entry in FAT, dx = cluster mod 2

    mov si, buffer
    add si, ax
    mov ax, [ds:si]                     ; read entry from FAT table at index ax

    or dx, dx
    jz .even

.odd:
    shr ax, 4
    jmp .next_cluster_after

.even:
    and ax, 0x0FFF

.next_cluster_after:
    cmp ax, 0x0FF8                      ; end of chain
    jae .read_finish

    mov [kernel_cluster], ax
    jmp .load_kernel_loop

.read_finish:
    
    ; jump to our kernel
    mov dl, [ebr_drive_number]          ; boot device in dl

    mov ax, KERNEL_LOAD_SEGMENT         ; set segment registers
    mov ds, ax
    mov es, ax

    jmp KERNEL_LOAD_SEGMENT:KERNEL_LOAD_OFFSET

    jmp wait_key_and_reboot             ; should never happen

    cli                                 ; disable interrupts, this way CPU can't get out of "halt" state
    hlt


;
; Обработчики ошибок
;

floppy_error:
    mov si, msg_read_error
    call puts

    jmp wait_key_and_reboot

kernel_not_found_error:
    mov si, msg_kernel_not_found
    call puts

    jmp wait_key_and_reboot

wait_key_and_reboot:
    mov ah, 0
    int 16h
    jmp 0FFFFh:0

.halt:
    cli
    hlt


;
; Вывод строки на экран
;
; Параметры:
; - ds:si: адрес начала строки
;

puts:
    push si
    push ax
    push bx

.loop:
    mov ah, 0Eh
    mov bh, 0
    lodsb
    cmp al, 0
    je .done
    int 10h

    jmp .loop

.done:
    pop bx
    pop ax
    pop si
    ret



;
; Конвертация LDA адреса в CHS адрес
;
; Параметры:
; - ax: LBA адрес
;
; Выход:
; - cx [bits 0-5]: номер сектора
; - cx [bits 6-15]: цилиндр
; - dh: головка
;

lba_to_chs:
    push ax
    push dx

    xor dx, dx                              ; dx = 0
    div word [bdb_sectors_per_track]        ; ax = LBA / SectorsPerTrack
                                            ; dx = LBA % SectorsPerTrack

    inc dx                                  ; dx = LBA % SectorsPerTrack + 1 = сектор
    mov cx, dx                              ; cx = сектор

    xor dx, dx                              ; dx = 0
    div word [bdb_heads]                    ; ax = (LBA / SectorsPerTrack) / Heads = цилиндр
                                            ; dx = (LBA / SectorsPerTrack) % Heads = головка

    mov ch, al                              ; ch = цилиндр (младшие 8 бит)
    shl ah, 6
    or cl, ah                               ; Поместим старшие 2 бита в CL
    mov dh, dl                              ; dh = головка

    pop ax
    mov dl, al                              ; Восстановим DL
    pop ax
    ret



;
; Чтение сектора с диска
;
; Параметры:
; - ax: LBA адрес
; - cl: номер сектора
; - dl: номер диска
; - es:bx: адрес ячейки в которую будут записаны данные с диска
;

disk_read:
    push ax
    push bx
    push cx
    push dx
    push di

    push cx
    call lba_to_chs
    pop ax

    mov ah, 02h
    mov di, 3

.retry:
    pusha
    stc
    int 13h
    jnc .done

    popa
    call disk_reset

    dec di
    test di, di
    jnz .retry

.fail:
    jmp floppy_error

.done:
    popa

    pop di
    pop dx
    pop cx
    pop bx
    pop ax
    ret



;
; Сброс контроллера диска
;
; Параметры:
; - dl: номер диска
;

disk_reset:
    pusha

    mov ah, 0
    stc
    int 13h
    jc floppy_error

    popa
    ret


msg_loading:                db "Loading...", ENDL, 0
msg_read_error:             db "Read from disk failed!", ENDL, 0
msg_kernel_not_found:       db "KERNEL.BIN file not found!", ENDL, 0
file_kernel_bin:            db "KERNEL  BIN"
kernel_cluster:             dw 0

KERNEL_LOAD_SEGMENT:        equ 0x2000
KERNEL_LOAD_OFFSET:         equ 0

times 510 - ($-$$) db 0
dw 0xAA55

buffer: