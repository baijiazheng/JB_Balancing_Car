// [EN] Header Guard: Prevents multiple inclusion of this header file during compilation.
// [CN] 头文件卫士：防止在编译过程中多次包含此头文件，避免重复定义错误。
#ifndef MPU6050_H
#define MPU6050_H

#include<Arduino.h>

#define MPU6050_ADDR 0x68
#define SMPLRT_DIV   0x19
#define CONFIG       0x1A
#define GYRO_CONFIG  0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1   0x6B
#define ACCEL_XOUT_H 0x3B

// [EN] Function prototypes and global variables go here.
// [CN] 函数原型和全局变量声明写在这里。
extern int16_t accelX, accelY, accelZ;
extern int16_t gyroX, gyroY, gyroZ;
extern int16_t tempRaw;
extern float pitchAngle; // 最终融合的倾角
extern float gyroRateX;  // 真实的角速度(度/秒)，后面 PID 的微分项要用到

void MPU6050_Init();
void MPU6050_Read_RAW();
void MPU6050_Calculate_Angle(float dt);// [EN] Function to calculate the real angle using Complementary Filter/ [CN] 使用互补滤波计算真实角度的函数

#endif // MPU6050_H
