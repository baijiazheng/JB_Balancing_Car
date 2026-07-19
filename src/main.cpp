#include <Arduino.h>
#include "MPU6050.h"
#include "Control.h"
#include "Bluetooth.h"
#include "Timer1.h"
#include "HW_CONFIG.h"
#include "Motor.h"
#include "Encoder.h"


// ================= 主系统入口 =================
void setup() {
    Hardware_Init(); // [EN]Initialize hardware peripherals/[CH]初始化硬件外设
    //Telemetry_CONFIG(); // [EN]Configure Bluetooth module/[CH]配置蓝牙模块
    Telemetry_Init();
    MPU6050_Init();
    Encoder_Init();
    Timer1_Init();// [EN]Initialize Timer 1/[CH]初始化定时器 1 
    //Motor_Diagnostic_Test();
}

// ================== 主循环 ==================
    void loop() {
        if(flag_5ms){
            flag_5ms = false; // [EN]Clear the 5ms flag/[CH]清除 5ms 标志位

            // 1. [EN] Perception: Read and fuse sensor data (Takes ~0.5ms)
            // 1. [CN] 感知层：读取并融合传感器姿态数据 (耗时约 0.5ms)
            MPU6050_Read_RAW();
            MPU6050_Calculate_Angle(0.005);

            // int32_t speed_left = Get_Encoder_Speed_L();
            // int32_t speed_right = Get_Encoder_Speed_R();
            // int32_t encoder_count_L = Get_Encoder_Count_L();
            // int32_t encoder_count_R = Get_Encoder_Count_R();

            // 2. [EN] Control: Standard PD calculation
            // 2. [CN] 控制层：标准的直立 PD 计算
            int16_t motor_out = Upright_PD_Control(pitchAngle, gyroRateX);

            // 3. [EN] Actuation: Output to motors
            // 3. [CN] 执行层：输出到电机
            if (abs(pitchAngle) > 45.0) {
                Set_Motor(0, 0);
                return; 
            }
            if(abs(motor_out)<20){
                motor_out=0;
            }
            Set_Motor(motor_out, motor_out);

            // ================= 3. 降频遥测层 (时间片切分) =================
            //Telemetry_Send_100ms(pitchAngle, gyroRateX);
            // Telemetry_Send_100ms(speed_left, speed_right);
        }

    // ================= 空闲时间：处理接收指令 =================
    char cmd = Telemetry_Receive_Cmd();
    if (cmd != '\0') { 
        // 如果收到了字符，这里处理遥控逻辑
        // if (cmd == 'F') { target_angle = 3.0; }
    }

}