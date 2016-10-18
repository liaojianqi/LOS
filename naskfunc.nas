[FORMAT "WCOFF"]
[INSTRSET "i486p"]      
[BITS 32]
[FILE "naskfunc.nas"]

GLOBAL  _io_hlt, _io_cli, _io_sti, _io_stihlt
GLOBAL  _io_in8,  _io_in16,  _io_in32
GLOBAL  _io_out8, _io_out16, _io_out32
GLOBAL  _io_load_eflags, _io_store_eflags  
GLOBAL  _load_gdtr, _load_idtr   
GLOBAL  _asm_inthandler21,_asm_inthandler2c
GLOBAL  _io_load_cr0,_io_store_cr0,_memtest_sub

EXTERN  _inthandler21,_inthandler2c,_inthandler27



[SECTION .text]     

_io_hlt:    ; void io_hlt(void);
        HLT
        RET

_io_cli:    ; void io_cli(void);
        CLI
        RET

_io_sti:    ; void io_sti(void);
        STI
        RET

_io_stihlt: ; void io_stihlt(void);
        STI
        HLT
        RET

_io_in8:    ; int io_in8(int port);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,0
        IN      AL,DX
        RET

_io_in16:   ; int io_in16(int port);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,0
        IN      AX,DX
        RET

_io_in32:   ; int io_in32(int port);
        MOV     EDX,[ESP+4]     ; port
        IN      EAX,DX
        RET

_io_out8:   ; void io_out8(int port, int data);
        MOV     EDX,[ESP+4]     ; port
        MOV     AL,[ESP+8]      ; data
        OUT     DX,AL
        RET

_io_out16:  ; void io_out16(int port, int data);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,[ESP+8]     ; data
        OUT     DX,AX
        RET

_io_out32:  ; void io_out32(int port, int data);
        MOV     EDX,[ESP+4]     ; port
        MOV     EAX,[ESP+8]     ; data
        OUT     DX,EAX
        RET

_io_load_eflags:    ; int io_load_eflags(void);
        PUSHFD      ; PUSH EFLAGS ‚Æ‚¢‚¤ˆÓ–¡
        POP     EAX
        RET

_io_store_eflags:   ; void io_store_eflags(int eflags);
        MOV     EAX,[ESP+4]
        PUSH    EAX
        POPFD       ; POP EFLAGS ‚Æ‚¢‚¤ˆÓ–¡
        RET
_load_gdtr:             ; void load_gdtr(int limit, int addr);
        MOV             AX,[ESP+4]              ; limit
        MOV             [ESP+6],AX
        LGDT    [ESP+6]
        RET

_load_idtr:             ; void load_idtr(int limit, int addr);
        MOV             AX,[ESP+4]              ; limit
        MOV             [ESP+6],AX
        LIDT    [ESP+6]
        RET

_asm_inthandler21:
        PUSH    ES 
        PUSH    DS
        PUSHAD
        MOV             EAX,ESP
        PUSH    EAX
        MOV             AX,SS
        MOV             DS,AX
        MOV             ES,AX
        CALL    _inthandler21
        POP             EAX
        POPAD
        POP             DS
        POP             ES
        IRETD
_asm_inthandler2c:
        PUSH    ES 
        PUSH    DS
        PUSHAD
        MOV             EAX,ESP
        PUSH    EAX
        MOV             AX,SS
        MOV             DS,AX
        MOV             ES,AX
        CALL    _inthandler2c
        POP             EAX
        POPAD
        POP             DS
        POP             ES
        IRETD 
_asm_inthandler27:
        PUSH    ES
        PUSH    DS
        PUSHAD
        MOV             EAX,ESP
        PUSH    EAX
        MOV             AX,SS
        MOV             DS,AX
        MOV             ES,AX
        CALL    _inthandler27
        POP             EAX
        POPAD
        POP             DS
        POP             ES
        IRETD

_io_load_cr0:
        MOV eax,cr0
        ret
_io_store_cr0:
        mov eax,[esp+4]
        mov cr0,eax
        ret
_memtest_sub:
        push    ebx
        push    edx
        mov     ebx,[esp+8+4]
loop_fun:
        add     ebx,0ffch
        mov     edx,[ebx]
        mov     eax,0aa55aa55h
        mov     [ebx],eax
        mov     eax,0ffffffffh
        xor     [ebx],eax
        mov     eax,055aa55aah
        cmp     [ebx],eax
        jne     _memtest_sub_end
        mov     eax,0ffffffffh
        xor     [ebx],eax
        mov     eax,0aa55aa55h
        cmp     [ebx],eax
        jne     _memtest_sub_end
        mov     [ebx],edx
        add     ebx,1000h
        cmp     ebx,[esp+8+8]
        jbe     loop_fun
_memtest_sub_end:
        mov     eax,ebx
        pop     edx
        pop     ebx
        ret