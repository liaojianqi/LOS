#include "bootpack.h"

void HariMain(void){  
    //初始化gdt,idt
    init_gdtidt(); 
    //初始化调色板
    init_palette();
    //显示画面
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    init_screen(b_info);
    //显示变量值
    char s[20];
    int x=30,y=30;
    sprintf(s,"x=%d,y=%d",x,y);
    put_string(b_info->vram,b_info->scrnx,30,30,COL8_FFFFFF,s);
    //显示鼠标指针
    char mouse[16*16];
    init_mouse_cursor8(mouse,COL8_008484);
    putblock8_8(b_info->vram,b_info->scrnx,16,16,60,60,mouse);

    //CPU休眠
    for(;;){
        io_hlt();
    }
}



