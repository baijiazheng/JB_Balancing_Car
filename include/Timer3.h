#ifndef Timer3_h
#define Timer3_h

#include <Arduino.h>

// [EN] Volatile flag for 100us timing tick. Must be volatile because it's modified in an ISR.
// [CN] 用于 100us 定时的标志位。由于在中断中被修改，必须加 volatile 关键字防止编译器优化。
extern volatile bool flag_100us;


// [EN]Initialize Timer 3
//[CH]初始化定时器 3
void Timer3_Init(); 

// [EN]Timer 3 Compare Match A Interrupt Service Routine
//[CH]定时器 3 比较匹配 A 中断服务程序
ISR(TIMER3_COMPA_vect); 

#endif // Timer3_h