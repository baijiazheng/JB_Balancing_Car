// [EN] Encoder Driver Layer (Interrupt Based)
// [CN] 编码器驱动层 (基于硬件中断)

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

// [EN] Get the number of accumulated pulses since last read (and clear counter)
// [CN] 获取自上次读取以来积累的脉冲数 (读取后自动清零)
int32_t Get_Encoder_Speed_L();
int32_t Get_Encoder_Speed_R();
    
// [EN] Get the total accumulated pulse count since system start (does not clear counter)
// [CN] 获取自系统启动以来的总脉冲数 (不会清零)
int32_t Get_Encoder_Count_L();
int32_t Get_Encoder_Count_R();

// [EN] Initialize external interrupts for encoders
// [CN] 初始化编码器外部中断
void Encoder_Init();

#endif