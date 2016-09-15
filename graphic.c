#include "bootpack.h"

//初始化屏幕
void init_screen(struct BOOTINFO *b_info){
    char *vram=b_info->vram;
    int x=b_info->scrnx,y=b_info->scrny;
    boxfill8(vram, x, COL8_008484,  0,0,x-1, y - 29); //绿色部分
    boxfill8(vram, x, COL8_C6C6C6,  0,     y - 28, x -  1, y - 28);
    boxfill8(vram, x, COL8_FFFFFF,  0,     y - 27, x -  1, y - 27);
    boxfill8(vram, x, COL8_C6C6C6,  0,     y - 26, x -  1, y -  1);
    boxfill8(vram, x, COL8_848484, x - 47, y - 24, x -  4, y - 24);
    boxfill8(vram, x, COL8_848484, x - 47, y - 23, x - 47, y -  4);
    boxfill8(vram, x, COL8_FFFFFF, x - 47, y -  3, x -  4, y -  3);
    boxfill8(vram, x, COL8_FFFFFF, x -  3, y - 24, x -  3, y -  3);  
    put_string(vram,x,5,y-20,COL8_FF0000,"Welcome!");
    put_string(vram,x,x-37,y-20,COL8_840000,"LOS");
    put_string(vram,x,10,10,COL8_FFFFFF,"Welcome to LOS!"); 
    
}
//打印一个字符,x,y是起始地址，color是颜色,c是待打印字符
void put_char(char*vram,int x_size,int x,int y,char color,char c){
    extern char hankaku[4096];
    int i=0;
    for(i=0;i<16;i++){
        char d = hankaku[c*16+i];
        char *p = (char*)(vram+(y+i)*x_size+x);
        int j=0;
        for(j=0;j<8;j++){
            if(d&1){
                p[7-j]=color;
            }
            d>>=1;
        }
    }
}
void put_string(char*vram,int x_size,int x,int y,char color,char* s){
    int l = strlen(s);
    int i=0;
    for(i=0;i<l;i++){
        put_char(vram,x_size,x+i*8,y,color,s[i]);
    }
}
//bc是背景色
void init_mouse_cursor8(char*mouse,char bc){
    static char cursor[16][16]={
        "**************..",
        "*OOOOOOOOOOO*...",
        "*OOOOOOOOOO*....",
        "*OOOOOOOOO*.....",
        "*OOOOOOOO*......",
        "*OOOOOOO*.......",
        "*OOOOOOO*.......",
        "*OOOOOOOO*......",
        "*OOOO**OOO*.....",
        "*OOO*..*OOO*....",
        "*OO*....*OOO*...",
        "*O*......*OOO*..",
        "**........*OOO*.",
        "*..........*OOO*",
        "............*OO*",
        ".............***"
    };
    int x,y;
    for(y=0;y<16;y++){
        for(x=0;x<16;x++){
            if(cursor[x][y]=='*'){
                mouse[y*16+x]=COL8_000000;
            }else if(cursor[x][y]=='O'){
                mouse[y*16+x]=COL8_FFFFFF;
            }else{
                mouse[y*16+x]=bc;
            }
        }
    }
}
//打印鼠标指针
void putblock8_8(char *vram,int x_size,int pxsize,int pysize,int px0,int py0,char *mouse){
    int x,y;
    for(y=0;y<pysize;y++){
        for(x=0;x<pxsize;x++){
            vram[(py0+y)*x_size+px0+x] = mouse[y*pxsize+x];
        }
    }
}
//绘制矩形
//varm是起始显存，xsize是x有多少个像素点,c是颜色,x0,y0,x1,y1分别是左上角和右下角的x,y坐标
void boxfill8(unsigned char*vram,int xsize,unsigned char c,int x0,int y0,int x1,int y1){
    int x,y;
    for(x=x0;x<=x1;x++){
        for(y=y0;y<=y1;y++){
            vram[x+y*xsize]=c;
        }
    }
}
//初始化调色板
void init_palette(void){
    static unsigned char table_rgb[16 * 3] = {
        0x00, 0x00, 0x00,   /*  0:黑*/
        0xff, 0x00, 0x00,   /*  1:亮红*/
        0x00, 0xff, 0x00,   /*  2:亮绿 */
        0xff, 0xff, 0x00,   /*  3:亮黄 */
        0x00, 0x00, 0xff,   /*  4:亮蓝 */
        0xff, 0x00, 0xff,   /*  5:亮紫 */
        0x00, 0xff, 0xff,   /*  6:浅亮蓝 */
        0xff, 0xff, 0xff,   /*  7:白*/
        0xc6, 0xc6, 0xc6,   /*  8:亮灰 */
        0x84, 0x00, 0x00,   /*  9:暗红*/
        0x00, 0x84, 0x00,   /* 10:暗绿 */
        0x84, 0x84, 0x00,   /* 11:暗黄 */
        0x00, 0x00, 0x84,   /* 12:暗青 */
        0x84, 0x00, 0x84,   /* 13:暗紫 */
        0x00, 0x84, 0x84,   /* 14:浅暗蓝 */
        0x84, 0x84, 0x84    /* 15:暗灰 */
    };
    set_palette(0, 15, table_rgb);
    return;
}
//设定调色板
void set_palette(int start, int end, unsigned char *rgb){
    int i, eflags;
    eflags = io_load_eflags();//记录中断许可标志的置
    io_cli();                //禁止中断
    io_out8(0x03c8, start);
    for (i = start; i <= end; i++) {
        io_out8(0x03c9, rgb[0] / 4);
        io_out8(0x03c9, rgb[1] / 4);
        io_out8(0x03c9, rgb[2] / 4);
        rgb += 3;
    }
    io_store_eflags(eflags); //恢复中断
    return;
}