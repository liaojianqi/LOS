void io_hlt(void);
void io_cli(void); 
void io_sti(void);
void io_out8(int port, int data); 
unsigned char io_in8(int port); 
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int,int);
void load_idtr(int,int); 
 
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
//设置PIC
void init_pic(void);
void asm_inthandler21(void);//中断处理程序
void asm_inthandler27(void);
void asm_inthandler2c(void);
#define PIC0_ICW1       0x0020
#define PIC0_OCW2       0x0020
#define PIC0_IMR        0x0021
#define PIC0_ICW2       0x0021
#define PIC0_ICW3       0x0021
#define PIC0_ICW4       0x0021
#define PIC1_ICW1       0x00a0
#define PIC1_OCW2       0x00a0
#define PIC1_IMR        0x00a1
#define PIC1_ICW2       0x00a1
#define PIC1_ICW3       0x00a1
#define PIC1_ICW4       0x00a1


//一个内存段
/*要保存的信息有：开始地址，段长，权限信息*/
struct SEGMENT_DESCRIPTOR {
    //limit：段的字节数-1.即段的范围：[base,base+lime]
    //base:4字节,limit:20位,access_right:12位
    short limit_low, base_low;
    char base_mid, access_right;
    char limit_high, base_high;
};

struct GATE_DESCRIPTOR {
    short offset_low, selector;
    char dw_count, access_right;
    short offset_high;
};

/* 绘图相关 */
void init_screen(struct BOOTINFO*);
void set_palette(int , int , unsigned char*);
void put_char(char* ,int ,int,int,char,char);
void put_string(char*vram,int x_size,int x,int y,char color,char* s);
void init_mouse_cursor8(char*mouse,char bc);
void putblock8_8(char *vram,int x_size,int pxsize,int pysize,int px0,int py0,char *mouse);
void boxfill8(unsigned char*vram,int xsize,unsigned char c,int x0,int y0,int x1,int y1);
void init_palette(void);
/* 键盘中断处理 */
struct KEY_BUFF{
    unsigned char data[32],sz;
};