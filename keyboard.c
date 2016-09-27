#include "bootpack.h"
/* 确定控制电路是否做好准备 */
void KBC_ready(){
    for(;;){
        if((io_in8(KBC_CONTROL) & 0x02) == 0)    //==号的优先级高于位运算,错了好久
            break;
    }
}
