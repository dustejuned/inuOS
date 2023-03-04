ORG 0
BITS 16

_start:
    jmp short start
    nop

times 33 db 0

handle_zero:
    mov ah, 0eh
    mov al, 'Z'
    mov bx, 0x00
    int 0x10
    iret

handle_one:
    mov ah, 0eh
    mov al, 'O'
    mov bx, 0x00
    int 0x10
    iret

start:
    jmp 0x7c0:step2

step2:
    cli ;clear interrupts
    mov ax, 0x7c0
    mov ds, ax
    mov es, ax
    mov ax, 0x00
    mov ss, ax
    mov sp, 0x7c0
    sti ;enable interrupts

    mov ah, 2 ;Read sector command
    mov al, 1 ;One sector to read
    mov ch, 0 ;Cylinder low eight bits
    mov cl, 2 ;Read sector 2
    mov dh, 0 ;Head Number
    mov bx, buffer ;Load Buffer
    int 0x13 ;Raise disk read interrupt
    jc error
    mov si, buffer
    call print
    jmp $

error:
    mov si, error_msg
    call print
    jmp $

print:
    mov bx, 0
.loop:
    lodsb
    cmp al, 0
    je .done
    call print_char
    jmp .loop
.done:
    ret

print_char:
    mov ah, 0eh
    int 0x10
    ret

error_msg: db 'Error, Unable to read sector!', 0

times 510-($ - $$) db 0
dw 0xAA55

buffer:
