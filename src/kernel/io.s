global io_sendbyte

io_sendbyte:
    mov al, [esp + 8] 
    mov dx, [esp + 4]
    out dx, al
    ret

global io_recvbyte

io_recvbyte:
    mov dx, [esp + 4]
    in al, dx
    ret

global io_wait

io_wait:
    jmp _io_wait1
_io_wait1:
    jmp _io_wait2
_io_wait2:
    ret
