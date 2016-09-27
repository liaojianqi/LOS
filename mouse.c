#include "bootpack.h"

/* 设置鼠标控制电路可用 */
void set_mouse_control_circle_enable(){
    KBC_ready();
    io_out8(KBC_CONTROL,0x60);//0x60是模式设定指令
    KBC_ready();
    io_out8(KBC_DATA,0x47);//0x47是设置鼠标电路可用
}
void set_mouse_enable(){  
    KBC_ready();
    io_out8(KBC_CONTROL,0xd4);//0x60是模式设定指令
    KBC_ready();
    io_out8(KBC_DATA,0xf4);//0x47是设置鼠标电路可用
}
int mouse_decode(struct MOUSE_DEC* mouse_info,unsigned char c){
    if(mouse_info->sz==0){
        if(c==0xfa){
            mouse_info->sz=1;
        }
        return 0;
    }
    if(mouse_info->sz==1){
        if((c&0xc8)==0x08){//第一字节正确
            mouse_info->buf[0]=c;        
            mouse_info->sz=2;
        }
        return 0;
    }
    mouse_info->buf[mouse_info->sz-1]=c;
    mouse_info->sz++;
    if(mouse_info->sz==4){
        mouse_info->sz=1;
        //获取鼠标数据
        mouse_info->btn = mouse_info->buf[0]&0x07;  //低三位是鼠标点击状态
        mouse_info->x=mouse_info->buf[1];
        mouse_info->y=mouse_info->buf[2];
        if((mouse_info->buf[0]&0x10)!=0){
            mouse_info->x |= 0xffffff00;
        }
        if((mouse_info->buf[0]&0x20)!=0){
            mouse_info->y |= 0xffffff00;
        }
        mouse_info->y=-mouse_info->y;
        return 1;
    }
    return 0;
}