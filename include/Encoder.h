// [EN] Encoder Driver Layer (Interrupt Based)
// [CN] 编码器驱动层 (基于硬件中断)

#ifndef ENCODER_H
#define ENCODER_H

#include <Arduino.h>

#define ENC_Hz 20 // [EN] Encoder reading frequency (Hz) / [CN] 编码器读取频率 (Hz)
#define ENCODER_LEFT_DIRECTION  1 // [EN] Direction of the left encoder (1 for normal, -1 for reversed) / [CN] 左编码器的方向 (1 为正常，-1 为反向)
#define ENCODER_RIGHT_DIRECTION -1 // [EN] Direction of the right encoder (1 for normal, -1 for reversed) / [CN] 右编码器的方向 (1 为正常，-1 为反向)

extern int32_t speed_car; // [EN] Average speed of the car / [CN] 小车的平均速度

// [EN] Get the number of accumulated pulses since last read (and clear counter)
// [CN] 获取自上次读取以来积累的脉冲数 (读取后自动清零)
int32_t Get_Encoder_Speed_L();
int32_t Get_Encoder_Speed_R();
    
// [EN] Get the total accumulated pulse count since system start (does not clear counter)
// [CN] 获取自系统启动以来的总脉冲数 (不会清零)
int32_t Get_Encoder_Tick_L();
int32_t Get_Encoder_Tick_R();

// [EN] Get the number of interrupts that have occurred for each encoder
// [CN] 获取每个编码器发生的中断次数
uint32_t Get_ISR_Count_L();
uint32_t Get_ISR_Count_R();

// [EN] Get the number of high and low states for each encoder's B phase
// [CN] 获取每个编码器 B 相的高电平和低电平状态次数
uint32_t Get_B_HIGH_L_Count();
uint32_t Get_B_LOW_L_Count();
uint32_t Get_B_HIGH_R_Count();
uint32_t Get_B_LOW_R_Count();

// [EN] Initialize external interrupts for encoders
// [CN] 初始化编码器外部中断
void Encoder_Init();

// [EN] Get the average speed of the car (average of left and right wheel speeds)
// [CN] 获取小车的平均速度 (左右轮速度的平均值)
void Get_Speed();

#endif