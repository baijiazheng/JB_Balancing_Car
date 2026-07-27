#include "Encoder_Test.h"
#include "Encoder.h"
#include "HW_CONFIG.h"
#include "Timer1.h"
#include "Motor.h"

#define SPEED_TEST 50 // [EN] Set the speed for the test / [CN] 设置测试的速度

void Encoder_Test_Init() {
    Serial.begin(115200); // [EN] Initialize Serial Monitor / [CN] 初始化串口监视器
    Hardware_Init(); // [EN]Initialize hardware peripherals/[CH]初始化硬件外设
    Encoder_Init();
    Timer1_Init();
    Set_Motor(SPEED_TEST, SPEED_TEST); // [EN] Set the motors to a constant speed for testing / [CN] 设置电机为恒定速度进行测试
}

void Encoder_Test() {
    if (flag_5ms) { // [EN] Check if 5ms flag is set / [CN] 检查 5ms 标志位是否被置位
        flag_5ms = false; // [EN] Clear the flag / [CN] 清除标志位
        static int8_t Encoder_Test_Counter = 0; // [EN] Counter for the number of tests / [CN] 测试次数计数器
        ++Encoder_Test_Counter;

        if (Encoder_Test_Counter >= 20){
            // [EN] Read encoder speeds and positions
            // [CN] 读取编码器速度和位置
            int32_t speed_left = Get_Encoder_Speed_L();
            int32_t speed_right = Get_Encoder_Speed_R();
            int32_t encoder_tick_L = Get_Encoder_Tick_L();
            int32_t encoder_tick_R = Get_Encoder_Tick_R();

            // [EN] Print the results to Serial Monitor
            // [CN] 将结果打印到串口监视器
            // Serial.print("Speed Left: ");
            // Serial.print(speed_left);
            // Serial.print(" | Speed Right: ");
            // Serial.print(speed_right);
            // Serial.print(" | Tick Left: ");
            // Serial.print(encoder_tick_L);
            // Serial.print(" | Tick Right: ");
            // Serial.println(encoder_tick_R);
            // Serial.print("Average Speed of Car: ");
            // Serial.println(speed_car);
            // Serial.print("Isr_L_count: ");
            // Serial.println(Get_ISR_Count_L());
            // Serial.print("Isr_R_count: ");
            // Serial.println(Get_ISR_Count_R());

            Serial.print("B_HIGH_L_count: ");
            Serial.print(Get_B_HIGH_L_Count());
            Serial.print(" | B_LOW_L_count: ");
            Serial.print(Get_B_LOW_L_Count());
            Serial.print(" | B_HIGH_R_count: ");
            Serial.print(Get_B_HIGH_R_Count());
            Serial.print(" | B_LOW_R_count: ");
            Serial.println(Get_B_LOW_R_Count());

            // Serial.print(digitalRead(PIN_ENC_L_A));
            // Serial.print(" ");
            // Serial.println(digitalRead(PIN_ENC_R_B));

            Encoder_Test_Counter = 0; // [EN] Reset the counter / [CN] 重置计数器
        }
    }
    
}