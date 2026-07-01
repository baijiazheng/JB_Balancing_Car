#ifndef Motor_H
#define Motor_H

#include <Arduino.h>

#define Motor_MAX_PWM 200 // [EN] Maximum PWM value for motor control / [CH] 电机控制的最大 PWM 值

// [EN] Set Motor speeds (-255 to 255)
// [CN] 设置电机速度，正数正转，负数反转，0为刹车
void Set_Motor(int16_t speed_L, int16_t speed_R);

// [EN] Motor Diagnostic Sweep Routine
// [CN] 电机扫频压测函数 (开机执行一次)
void Motor_Diagnostic_Test() ;

#endif // Motor_H