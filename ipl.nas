;读取10个柱面    
    CYLS EQU    10
    ORG     0x7c00

    JMP     entry
    DB      0x90
    DB      "HELLOIPL"
    DW      512
    DB      1
    DW      1
    DB      2
    DW      224
    DW      2880
    DB      0xf0
    DW      9
    DW      18
    DW      2
    DD      0
    DD      2880
    DB      0,0,0x29
    DD      0xffffffff
    DB      "HELLO-OS   "
    DB      "FAT12   "
    RESB    18
entry:
    mov ax,0
    mov ss,ax
    mov sp,0x7c00
    mov ds,ax
    mov es,ax

    mov CH,0    ;柱面号
    mov DH,0    ;磁头号
    mov CL,2    ;扇区
    mov AX,0x0820
    mov ES,AX   ;地址

read_single_sector:
    mov AH,0x02
    mov al,1    ;扇区数
    mov bx,0    ;地址
    mov dl,0    ;驱动器号
    int 0x13
    ;地址增加
    mov AX,ES
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
    MOV [0x0ff0],ch         ;必须写，在asmhead.nas中用到了
    jmp 0xc200

    RESB    0x7dfe-$
    DB      0x55, 0xaa
