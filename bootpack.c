#include "bootpack.h"
#include <stdio.h>

extern struct KEY_BUFF key_buff,mouse_buff; 
struct MOUSE_DEC{
    unsigned char buf[3],sz;
    int x,y,btn;
};
struct MOUSE_DEC mouse_info;
int mouse_decode(unsigned char c){
    if(mouse_info.sz==0){
        if(c==0xfa){
            mouse_info.sz=1;
        }
        return 0;
    }
    if(mouse_info.sz==1){
        if((c&0xc8)==0x08){//第一字节正确
            mouse_info.buf[0]=c;        
            mouse_info.sz=2;
        }
        return 0;
    }
    mouse_info.buf[mouse_info.sz-1]=c;
    mouse_info.sz++;
    if(mouse_info.sz==4){
        mouse_info.sz=1;
        //获取鼠标数据
        mouse_info.btn = mouse_info.buf[0]&0x07;  //低三位是鼠标点击状态
        mouse_info.x=mouse_info.buf[1];
        mouse_info.y=mouse_info.buf[2];
        if((mouse_info.buf[0]&0x10)==0){
            mouse_info.x |= 0xffffff00;
        }
        if((mouse_info.buf[0]&0x20)==0){
            mouse_info.y |= 0xffffff00;
        }
        mouse_info.y=-mouse_info.y;
        return 1;
    }
    return 0;
}
void HariMain(void){   
    //初始化gdt,idt
    init_gdtidt(); 
    //设定PIC
    init_pic();
    //开启CPU接收外来中断
    io_sti();
    //设置IMR，开启中断
    io_out8(PIC0_IMR,  0xf9  ); /* 11111001 启用键盘中断*/
    io_out8(PIC1_IMR,  0xef  ); /* 11101111 启用鼠标中断 */
    //设置缓冲区
    unsigned char key_data[32];
    unsigned char mouse_data[128];
    init_fifo(&key_buff,32,key_data);
    init_fifo(&mouse_buff,128,mouse_data);
    // 设置鼠标控制电路可用
    set_mouse_control_circle_enable();
    //初始化调色板
    init_palette(); 
    //显示画面
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    init_screen(b_info);
   //显示鼠标指针
    char mouse[16*16];
    init_mouse_cursor8(mouse,COL8_008484);
    int x=60,y=60;
    putblock8_8(b_info->vram,b_info->scrnx,16,16,60,60,mouse);
    //设置鼠标可用
    set_mouse_enable();  
    for (;;) {
        io_cli();
        if(!has_next(&key_buff) && !has_next(&mouse_buff)){
            io_sti();
            io_hlt();
        }else{
            if(has_next(&mouse_buff)){
                unsigned char c = pop(&mouse_buff);
                io_sti();
                if(mouse_decode(c)==1){
                    //输出
                    // char s[20];
                    // sprintf(&s[0],"%02X",mouse_info.buf[0]);
                    // sprintf(&s[2],"%02X",mouse_info.buf[1]);
                    // sprintf(&s[4],"%02X",mouse_info.buf[2]);
                    // boxfill8(b_info->vram,b_info->scrnx,COL8_000000,0,0, 32*8-1, 15);
                    // put_string(b_info->vram,b_info->scrnx,0,0,COL8_FFFFFF,s);
                    //void putblock8_8(char *vram,int x_size,int pxsize,int pysize,int px0,int py0,char *mouse)
                    char s[20];
                    boxfill8(b_info->vram,b_info->scrnx,COL8_008484,x,y,x+16,y+16);//隐藏上一鼠标
                    boxfill8(b_info->vram,b_info->scrnx,COL8_008484,0,0,79,16);//隐藏上一坐标
                    x+=mouse_info.x;
                    y+=mouse_info.y;
                    if(x<0) x=0;
                    if(y<0) y=0;
                    if(x>b_info->scrnx-16) x=b_info->scrnx-16;
                    if(y>b_info->scrny-16) y=b_info->scrny-16;
                    sprintf(&s[0],"%4d",x);
                    sprintf(&s[4],"%4d",y);
                    put_string(b_info->vram,b_info->scrnx,0,0,COL8_FFFFFF,s);//描画坐标
                    putblock8_8(b_info->vram,b_info->scrnx,16,16,x,y,mouse);//描画鼠标
                }
            }
            if(has_next(&key_buff)){
                unsigned char s[4];
                sprintf(s,"%02X",pop(&key_buff));
                io_sti();
                boxfill8(b_info->vram,b_info->scrnx,COL8_000000,0,16, 32*8-1, 31);
                put_string(b_info->vram,b_info->scrnx,0,16,COL8_FFFFFF,s);   
            }
        }
    }
}



