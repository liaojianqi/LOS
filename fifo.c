#include "bootpack.h"

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