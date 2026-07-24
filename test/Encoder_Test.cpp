#include "Encoder_Test.h"
#include "Encoder.h"
#include "HW_CONFIG.h"

void Encoder_Test_Init() {
    Hardware_Init(); // [EN]Initialize hardware peripherals/[CH]初始化硬件外设
    Encoder_Init();
}

void Encoder_Test() {
    // [EN] Read encoder speeds and positions
    // [CN] 读取编码器速度和位置
    int32_t speed_left = Get_Encoder_Speed_L();
    int32_t speed_right = Get_Encoder_Speed_R();
    int32_t encoder_pos_L = Get_Encoder_Position_L();
    int32_t encoder_pos_R = Get_Encoder_Position_R();
    int32_t speed_car = (speed_left + speed_right) / 2; // [EN] Average speed of the car / [CN] 小车的平均速度
    
    // [EN] Print the results to Serial Monitor
    // [CN] 将结果打印到串口监视器
    Serial.print("Speed Left: ");
    Serial.print(speed_left);
    Serial.print(" | Speed Right: ");
    Serial.print(speed_right);
    Serial.print(" | Position Left: ");
    Serial.print(encoder_pos_L);
    Serial.print(" | Position Right: ");
    Serial.println(encoder_pos_R);
    Serial.print("Average Speed of Car: ");
    Serial.println(speed_car);

    delay(1000); // [EN] Delay for 1 second before next reading / [CN] 延迟 1 秒后再读取
}