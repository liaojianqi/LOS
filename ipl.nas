;读取10个柱面    
    CYLS EQU    10
    ORG     0x7c00

    JMP     entry
    db      0x90,"HELLOIPL"
    dw      512
    db      1
    dw      1
    db      2
    dw      224
    dw      2880
    db      0xf0
    dw      9
    dw      18
    dw      2
    dd      0
    dd      2880
    db      0,0,0x29
    dd      0xffffffff
    db      "HELLO-OS   "
    db      "FAT12   "
    resb    18
entry:
    mov ax,0
    mov ss,ax
    mov sp,0x7c00
    mov ds,ax
    mov es,ax

    mov ch,0    ;柱面号
    mov dh,0    ;磁头号
    mov cl,2    ;扇区
    mov ax,0x0820
    mov es,ax   ;地址

read_single_sector:
    mov ah,0x02
    mov al,1    ;扇区数
    mov bx,0    ;地址
    mov dl,0    ;驱动器号
    int 0x13
    ;地址增加
    mov ax,es
    add ax,0x0020
    mov es,ax
    add cl,1
    ;判断
    cmp cl,18
    ja  sector_end  ;读完18个扇区
    jmp read_single_sector

sector_end:
    cmp dh,1
    je  head_end
    mov dh,1
    mov cl,1
    jmp read_single_sector
head_end:
    add ch,1
    cmp ch,CYLS
    je  fin
    mov dh,0
    mov cl,1
    jmp read_single_sector
fin:
    mov [0x0ff0],ch         ;必须写，在asmhead.nas中用到了
    jmp 0x0c20:0

    resb    0x7dfe-$
    db      0x55, 0xaa
