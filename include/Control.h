// [EN] Header Guard for Control module
// [CN] 控制模块头文件卫士
#ifndef CONTROL_H
#define CONTROL_H

#include <Arduino.h>

// [EN] Global variables for PID tuning parameters
// [CN] 用于 PID 调参的全局变量 (设为 extern 方便未来通过蓝牙或串口动态修改)
extern float Kp_Upright;
extern float Kd_Upright;

// [EN] Target mechanical zero angle (Center of gravity)
// [CN] 机械零点目标角度 (重心绝对垂直地面的角度，通常不是绝对的 0 度，需要后期实测)
extern float Target_Angle;

// [EN] Function prototype for Upright PD Control
// [CN] 直立环 PD 控制函数原型
// 参数 pitch: 当前融合倾角 | 参数 gyro_rate: 当前陀螺仪角速度
int16_t Upright_PD_Control(float pitch, float gyro_rate);

#endif // CONTROL_H