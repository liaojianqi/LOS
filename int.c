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
/*IRQ1中断：键盘中断*/
void inthandler21(){
    struct BOOTINFO *b_info = (struct BOOTINFO*)0x0ff0;
    boxfill8(b_info->vram,b_info->scrnx,COL8_000000,0,0, 32*8-1, 15);
    put_string(b_info->vram,b_info->scrnx,0,0,COL8_FFFFFF,"keyboard is been pressed!");
    //CPU休眠
    for(;;){
        io_hlt();
    }
}
/*IRQ12中断：鼠标中断*/
void inthandler2c(){
    //CPU休眠
    for(;;){
        io_hlt();
    }
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