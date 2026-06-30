#ifndef HW_CONFIG_H
#define HW_CONFIG_H

#include <Arduino.h>
// ==========================================
// [EN] 1. Pin Mapping (Physical to Logical)
// [CN] 1. 引脚映射 (物理引脚 -> 逻辑名称)
//  注意：你需要根据你的 PCB 原理图修改以下具体数字！
// ==========================================

// --- Motor Drive (TB6612) Pins ---
constexpr uint8_t PIN_MOTOR_L_PWM = 8;  // 左轮速度控制
constexpr uint8_t PIN_MOTOR_L_IN1 = 4;  // 左轮方向 1
constexpr uint8_t PIN_MOTOR_L_IN2 = 5;  // 左轮方向 2

constexpr uint8_t PIN_MOTOR_R_PWM = 9;  // 右轮速度控制
constexpr uint8_t PIN_MOTOR_R_IN1 = 6;  // 右轮方向 1
constexpr uint8_t PIN_MOTOR_R_IN2 = 7;  // 右轮方向 2

// --- Encoder Pins ---
constexpr uint8_t PIN_ENC_L_A = 2;     // 左编码器 A 相 (必须是外部中断引脚)
constexpr uint8_t PIN_ENC_L_B = 22;     // 左编码器 B 相
constexpr uint8_t PIN_ENC_R_A = 3;     // 右编码器 A 相 (必须是外部中断引脚)
constexpr uint8_t PIN_ENC_R_B = 23;     // 右编码器 B 相



// ==========================================
// [EN] 2. Hardware Initialization Function
// [CN] 2. 硬件初始化函数声明
// ==========================================
void Hardware_Init();

#endif // HW_CONFIG_H

