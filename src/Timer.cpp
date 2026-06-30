#include "Timer1.h"

volatile bool flag_5ms = false; // [EN]5ms timing tick flag/[CH]5ms 定时标志位

// ================= Timer 1 初始化 (CTC 模式[比较匹配自动清零], 200Hz) =================
void Timer1_Init() {
    cli(); // [EN]Disable global interrupts/[CH]禁用全局中断

    TCCR1A = 0;          // [EN]normal operation/[CH]正常工作
    TCCR1B = 0;          // [EN]clear register/[CH]清零寄存器
    TCNT1 = 0;           // [EN]initialize counter/[CH]初始化计数器
    
    // 16MHz / (64 * 200Hz) - 1 = 1249
    OCR1A = 1249;         // [EN]Set compare match register for 200Hz increments/[CH]设置比较匹配寄存器，200Hz

    // [EN]Set CTC mode/[CH]设置 CTC 模式
    TCCR1B |= (1 << WGM12);

    // [EN]Set CS11 and CS10 bits for 64 prescaler/[CH]设置 CS11 和 CS10 位，预分频 64
    TCCR1B |= (1 << CS11) | (1 << CS10);

    // [EN]Enable timer compare interrupt/[CH]使能定时器比较中断
    TIMSK1 |= (1 << OCIE1A);

    sei(); // [EN]Enable global interrupts/[CH]启用全局中断
}

// ================= Timer 1 中断服务程序 =================
ISR(TIMER1_COMPA_vect) {
    flag_5ms = true; // [EN]Set the 5ms flag/[CH]设置 5ms 标志位(ISR越短越好)
}