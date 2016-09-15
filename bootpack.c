#include <stdio.h>

void io_hlt(void);
void init_gdtidt(void);

//设定好色号后的新颜色
#define COL8_000000     0
#define COL8_FF0000     1
#define COL8_00FF00     2
#define COL8_FFFF00     3
#define COL8_0000FF     4
#define COL8_FF00FF     5
#define COL8_00FFFF     6
#define COL8_FFFFFF     7
#define COL8_C6C6C6     8
#define COL8_840000     9
#define COL8_008400     10
#define COL8_848400     11
#define COL8_000084     12
#define COL8_840084     13
#define COL8_008484     14
#define COL8_848484     15

//保存启动初始信息的结构体(画面模式信息)
struct BOOTINFO{
    char cyls,leds,vmode,reserve;//柱面数，键盘状态，画面模式，
    short scrnx,scrny;
    char*vram;
};
//一个内存段
/*要保存的信息有：开始地址，段长，权限信息*/
struct SEGMENT_DESCRIPTOR {
    //limit：段的字节数-1.即段的范围：[base,base+lime]
    //base:4字节,limit:3字节,access_right:1字节
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

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



