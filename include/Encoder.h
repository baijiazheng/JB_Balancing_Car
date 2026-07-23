// [EN] Encoder Driver Layer (Interrupt Based)
// [CN] 编码器驱动层 (基于硬件中断)

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#define ENC_Hz 20 // [EN] Encoder reading frequency (Hz) / [CN] 编码器读取频率 (Hz)

extern int32_t speed_car; // [EN] Average speed of the car / [CN] 小车的平均速度

// [EN] Get the number of accumulated pulses since last read (and clear counter)
// [CN] 获取自上次读取以来积累的脉冲数 (读取后自动清零)
int32_t Get_Encoder_Speed_L();
int32_t Get_Encoder_Speed_R();
    
// [EN] Get the total accumulated pulse count since system start (does not clear counter)
// [CN] 获取自系统启动以来的总脉冲数 (不会清零)
int32_t Get_Encoder_Position_L();
int32_t Get_Encoder_Position_R();

// [EN] Initialize external interrupts for encoders
// [CN] 初始化编码器外部中断
void Encoder_Init();

// [EN] Get the average speed of the car (average of left and right wheel speeds)
// [CN] 获取小车的平均速度 (左右轮速度的平均值)
void Get_Speed();

#endif