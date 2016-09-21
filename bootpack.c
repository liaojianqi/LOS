#include "bootpack.h"
 
void HariMain(void){  
    //初始化gdt,idt
    init_gdtidt(); 
    //设定PIC
    init_pic();
    io_sti();//开启CPU接收外来中断
    //初始化调色板
    init_palette();
    //显示画面
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    init_screen(b_info);
   //显示鼠标指针
    char mouse[16*16];
    init_mouse_cursor8(mouse,COL8_008484);
    putblock8_8(b_info->vram,b_info->scrnx,16,16,60,60,mouse);
    //设置IMR，开启中断
    io_out8(PIC0_IMR,  0xf9  ); /* 11111001 启用键盘中断*/
    io_out8(PIC1_IMR,  0xef  ); /* 11101111 启用鼠标中断 */
    for (;;) {
        io_hlt();
    }
}



