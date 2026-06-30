#ifndef Timer1_h
#define Timer1_h

#include <Arduino.h>

// [EN] Volatile flag for 5ms timing tick. Must be volatile because it's modified in an ISR.
// [CN] 用于 5ms 定时的标志位。由于在中断中被修改，必须加 volatile 关键字防止编译器优化。
extern volatile bool flag_5ms;


// [EN]Initialize Timer 1
//[CH]初始化定时器 1
void Timer1_Init(); 

// [EN]Timer 1 Compare Match A Interrupt Service Routine
//[CH]定时器 1 比较匹配 A 中断服务程序
ISR(TIMER1_COMPA_vect); 

#endif // Timer1_h