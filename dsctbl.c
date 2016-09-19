#include "bootpack.h"

//初始化gdt和idt
void init_gdtidt(void){
    struct SEGMENT_DESCRIPTOR *gdt = (struct SEGMENT_DESCRIPTOR *) 0x00270000;//gdt保存的地址
    struct GATE_DESCRIPTOR    *idt = (struct GATE_DESCRIPTOR    *) 0x0026f800;//idt保存的地址
    int i;
    //共有8192个段
    for (i = 0; i < 8192; i++) {
        set_segmdesc(gdt + i, 0, 0, 0);
    }
    set_segmdesc(gdt + 1, 0xffffffff, 0x00000000, 0x4092);//高四位0100,低8位0x92(系统专用，可读写，不可执行)
    set_segmdesc(gdt + 2, 0x0007ffff, 0x00280000, 0x409a);//操作系统所在处，可执行，可读不可写
    load_gdtr(0xffff, 0x00270000);  //告诉GDTR,GDT表从哪里开始(后四字节)，总共有多少个字节(前两位)

    //共有256项
    for (i = 0; i < 256; i++) {
        set_gatedesc(idt + i, 0, 0, 0);
    }
    load_idtr(0x7ff, 0x0026f800);
}
/* 属性信息ar一部分存储在access_right中，一部分(ar的高4位)存储在limit_hight中 */
void set_segmdesc(struct SEGMENT_DESCRIPTOR *sd, unsigned int limit, int base, int ar){
    if (limit > 0xfffff) {
        ar |= 0x8000; /* G_bit = 1 */
        limit /= 0x1000;
    }
    sd->base_low     = base & 0xffff;
    sd->base_mid     = (base >> 16) & 0xff;
    sd->base_high    = (base >> 24) & 0xff;
    
    sd->limit_low    = limit & 0xffff;
    sd->limit_high   = ((limit >> 16) & 0x0f) | ((ar >> 8) & 0xf0);
    sd->access_right = ar & 0xff;
}

void set_gatedesc(struct GATE_DESCRIPTOR *gd, int offset, int selector, int ar){
    gd->offset_low   = offset & 0xffff;
    gd->selector     = selector;
    gd->dw_count     = (ar >> 8) & 0xff;
    gd->access_right = ar & 0xff;
    gd->offset_high  = (offset >> 16) & 0xffff;
}