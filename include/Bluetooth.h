// [EN] Header Guard for Control module
// [CN] 控制模块头文件卫士
#ifndef Bluetooth_H
#define Bluetooth_H

#include <Arduino.h>

// ==========================================
// [EN] 1. Hardware Serial Selection (Decoupling)
// [CN] 1. 硬件串口选择 (物理解耦)
// 物理映射: 引脚 14 (TX3) 和 15 (RX3) 对应 Serial3
// ==========================================
#define BT_SERIAL Serial3  // 已根据物理连线锁定为 Serial3
#define BT_BAUDRATE 115200   // 初始波特率 (在Telemetry_CONFIG中用 AT 指令改为 115200)

// ==========================================
// [EN] 2. Telemetry Functions
// [CN] 2. 遥测功能函数声明
// ==========================================

// [EN] Initialize Bluetooth hardware
// [CN] 初始化蓝牙硬件
void Telemetry_Init();

// [EN] Time-sliced transmission (Call this every 5ms, it will only send every 100ms)
// [CN] 时间片发送函数 (在5ms中断中调用，但底层会自动降频到100ms发送一次)
void Telemetry_Send_100ms(float pitch, int16_t motor_pwm);

// [EN] Check and parse incoming commands from phone
// [CN] 检查并解析手机端发来的遥控指令
char Telemetry_Receive_Cmd();

// [EN] Configure Bluetooth module (Optional, can be left empty)
// [CN] 配置蓝牙模块 (可选，留空即可)
void Telemetry_CONFIG();

#endif // Bluetooth_H