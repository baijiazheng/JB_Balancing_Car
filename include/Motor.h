#ifndef Motor_H
#define Motor_H

#include <Arduino.h>

#define Motor_MAX_PWM 200 // [EN] Maximum PWM value for motor control / [CH] 电机控制的最大 PWM 值

// [EN] Define whether the PWM signal is active low or active high
// [CH] 定义 PWM 信号是低电平有效还是高电平有效
constexpr bool PWM_ACTIVE_LOW = true;

// [EN] Write PWM value to motor pin, considering active low/high configuration
// [CH] 向电机引脚写入 PWM 值，考虑高低电平配置
void Motor_WritePWM(uint8_t pin, uint8_t duty);

// [EN] Set Motor speeds 
// [CN] 设置电机速度，正数正转，负数反转，0为刹车
void Set_Motor(int32_t speed_L, int32_t speed_R);

// [EN] Motor Diagnostic Sweep Routine
// [CN] 电机扫频压测函数 (开机执行一次)
void Motor_Diagnostic_Test() ;

// [EN] Filter the motor output to prevent sudden changes(Adaptive PWM Slew Rate Limiter)
// [CN] 对电机输出进行滤波，防止突变(自适应 PWM 变化率限制器)
int16_t Motor_Output_Filter(int16_t target_PWM,float pitchAngle);

// [EN] Drive the motor based on the filtered output
// [CN] 根据滤波后的输出驱动电机
void Drive_Motor(float pitchAngle,int16_t motor_out);

#endif // Motor_H