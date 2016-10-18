org 0xc200

BOTPAK  EQU     0x00280000      
DSKCAC  EQU     0x00100000      ; 待
DSKCAC0 EQU     0x00008000      ; 待


CYLS    EQU     0x0ff0          ;启动区
LEDS    EQU     0x0ff1          ;键盘状态
VMODE   EQU     0x0ff2          ;显示模式
SCRNX   EQU     0x0ff4          ;分辨率的X上有多少个像素点
SCRNY   EQU     0x0ff6          ;分辨率的Y上有多少个像素点
VRAM    EQU     0x0ff8          ;当前显示模式显存开始地址

;设置显示模式
mov    al,0x13         
mov    AH,0x00
int    0x10
mov    byte [VMODE],8  ;记录画面模式
mov    word [SCRNX],320
mov    word [SCRNY],200
mov    dword [VRAM],0x000a0000

; 使用bios获取键盘状态
mov     ah,0x02
int     0x16            ; keyboard BIOS
mov     [LEDS],al

;21号端口是PIC
;禁止一切PIC中断
mov     al,0xff
out     0x21,al
nop                     ; 如果连续执行out指令，有的机器会无法运行
out     0xa1,al

cli                     ; 禁止CPU级别的中断

;设置A20

        call    waitkbdout
        mov     al,0xd1
        out     0x64,AL
        call    waitkbdout
        mov     AL,0xdf         ; enable A20
        out     0x60,AL
        call    waitkbdout

; 切换到保护模式

[INSTRSET "i486p"]              

        LGDT    [GDTR0]         
        MOV     EAX,CR0
        AND     EAX,0x7fffffff  
        OR      EAX,0x00000001  
        MOV     CR0,EAX
        JMP     pipelineflush
pipelineflush:
        MOV     AX,1*8          
        MOV     DS,AX
        MOV     ES,AX
        MOV     FS,AX
        MOV     GS,AX
        MOV     SS,AX

;bootpack的传递

        MOV     ESI,bootpack    
        MOV     EDI,BOTPAK      
        MOV     ECX,512*1024/4
        CALL    memcpy


; 首先从启动扇区开始

        MOV     ESI,0x7c00      
        MOV     EDI,DSKCAC      
        MOV     ECX,512/4
        CALL    memcpy

;所有剩下的

        MOV     ESI,DSKCAC0+512 
        MOV     EDI,DSKCAC+512  
        MOV     ECX,0
        MOV     CL,BYTE [CYLS]
        IMUL    ECX,512*18*2/4  
        SUB     ECX,512/4       
        CALL    memcpy

;转到bootpack执行

        MOV     EBX,BOTPAK
        MOV     ECX,[EBX+16]
        ADD     ECX,3           ; ECX += 3;
        SHR     ECX,2           ; ECX /= 4;
        JZ      skip            ; “]‘—‚·‚é‚×‚«‚à‚Ì‚ª‚È‚¢
        MOV     ESI,[EBX+20]    ; “]‘—Œ³
        ADD     ESI,EBX
        MOV     EDI,[EBX+12]    ; “]‘—æ
        CALL    memcpy
skip:
        MOV     ESP,[EBX+12]    ; ƒXƒ^ƒbƒN‰Šú’l
        JMP     DWORD 2*8:0x0000001b

waitkbdout:
        IN       AL,0x64
        AND      AL,0x02
        JNZ     waitkbdout      ; AND‚ÌŒ‹‰Ê‚ª0‚Å‚È‚¯‚ê‚Îwaitkbdout‚Ö
        RET

memcpy:
        MOV     EAX,[ESI]
        ADD     ESI,4
        MOV     [EDI],EAX
        ADD     EDI,4
        SUB     ECX,1
        JNZ     memcpy          ; ˆø‚«ŽZ‚µ‚½Œ‹‰Ê‚ª0‚Å‚È‚¯‚ê‚Îmemcpy‚Ö
        RET
; memcpy‚ÍƒAƒhƒŒƒXƒTƒCƒYƒvƒŠƒtƒBƒNƒX‚ð“ü‚ê–Y‚ê‚È‚¯‚ê‚ÎAƒXƒgƒŠƒ“ƒO–½—ß‚Å‚à‘‚¯‚é

        ALIGNB  16
GDT0:
        RESB    8               ; ƒkƒ‹ƒZƒŒƒNƒ^
        DW      0xffff,0x0000,0x9200,0x00cf ; “Ç‚Ý‘‚«‰Â”\ƒZƒOƒƒ“ƒg32bit
        DW      0xffff,0x0000,0x9a28,0x0047 ; ŽÀs‰Â”\ƒZƒOƒƒ“ƒg32bitibootpack—pj

        DW      0
GDTR0:
        DW      8*3-1
        DD      GDT0

        ALIGNB  16
bootpack:
 