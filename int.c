#include "bootpack.h" 


void init_pic(){
    io_out8(PIC0_IMR,0xff); //禁止所有中断
    io_out8(PIC1_IMR,0xff); //禁止所有中断
 
    io_out8(PIC0_ICW1, 0x11  ); /* 1和4不考虑 */
    io_out8(PIC0_ICW2, 0x20  ); /* IRQ0-7由INT0x20到0x27处理 */
    io_out8(PIC0_ICW3, 1 << 2); /* 2号管脚与从PIC相连 */
    io_out8(PIC0_ICW4, 0x01  ); /* 1和4不考虑 */

    io_out8(PIC1_ICW1, 0x11  ); /* 1和4不考虑 */
    io_out8(PIC1_ICW2, 0x28  ); /* IRQ8-15由INT0x28到0x2f处理 */
    io_out8(PIC1_ICW3, 2     ); /* 与主PIC相连*/
    io_out8(PIC1_ICW4, 0x01  ); /* 1和4不考虑 */

    io_out8(PIC0_IMR,  0xfb  ); /* 11111011 禁止中断(PIC1除外) */
    io_out8(PIC1_IMR,  0xff  ); /* 11111111 禁止中断 */
}

void init_fifo(struct FIFO_BUFF *buff,int len,unsigned char * addr){
    buff->len = len;
    buff->data = addr;
    buff->next_w = 0;
    buff->next_r = 0;
}
int has_next(struct FIFO_BUFF * buff){
    if(buff->next_r!=buff->next_w) return 1;
    return 0;
}
void push(struct FIFO_BUFF* buff,unsigned char data){
    buff->data[buff->next_w++] = data;
    if(buff->next_w==buff->len){
        buff->next_w=0;
    }
}
unsigned char pop(struct FIFO_BUFF *buff){
    unsigned char data = buff->data[buff->next_r++];
    if(buff->next_r==buff->len){
        buff->next_r=0;
    }   
    return data;
}
/* 确定控制电路是否做好准备 */
#define KBC_DATA 0x0060
#define KBC_CONTROL 0x0064
void KBC_ready(){
    for(;;){
        if((io_in8(KBC_CONTROL) & 0x02) == 0)    //==号的优先级高于位运算,错了好久
            break;
    }
}
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
/*IRQ1中断：键盘中断*/
struct FIFO_BUFF key_buff;
#define PORT_KEYDAT 0x0060
void inthandler21(){
    io_out8(PIC0_OCW2,0x61);//通知IRQ1已处理完毕
    push(&key_buff,io_in8(PORT_KEYDAT)); //获取键盘输入信息
}
/*IRQ12中断：鼠标中断*/
void inthandler2c(){
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    io_out8(PIC0_OCW2,0x6c);//通知IRQc已处理完毕
    boxfill8(b_info->vram,b_info->scrnx,COL8_000000,0,0, 320-1, 15);
    put_string(b_info->vram,b_info->scrnx,0,0,COL8_FFFFFF,"mouse is been pressed!");
}
/*
    有些机器在初始化PIC时会产生一次IRQ7中断，不处理操作系统启动将失败
*/
void inthandler27(int *esp){
    io_out8(PIC0_OCW2, 0x67); 
    //CPU休眠
    for(;;){
        io_hlt();
    }
}