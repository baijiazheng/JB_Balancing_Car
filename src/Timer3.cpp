#include "Timer3.h"

volatile bool flag_100us = false ;

// ================= Timer 3 初始化 (100us 独立采样心跳) =================
void Timer3_Init() {
    cli(); // 关全局中断
    TCCR3A = 0;
    TCCR3B = 0;
    TCNT3  = 0;
    
    // Timer3 100us 中断配置 (16MHz / 64)
    OCR3A = 24; // (16000000 / (64 * 10000)) - 1 = 24 (100us)
    TCCR3B |= (1 << WGM32);              // CTC 模式
    TCCR3B |= (1 << CS31) | (1 << CS30); // 64 分频
    TIMSK3 |= (1 << OCIE3A);             // 允许 Timer3 比较匹配中断
    sei(); // 开全局中断
}

// --------------------------------------------------------
// [EN] Timer 3 ISR: Executes automatically every 100us
// [CN] Timer 3 中断服务函数：每 100us 自动静默执行，免疫 PWM 毛刺
// --------------------------------------------------------
ISR(TIMER3_COMPA_vect) {
    flag_100us = true ;
}