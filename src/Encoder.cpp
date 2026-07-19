#include "HW_CONFIG.h"
#include "Encoder.h"

volatile int32_t encoder_count_L = 0;
volatile int32_t encoder_count_R = 0;
volatile int32_t count_L = 0;
volatile int32_t count_R = 0;

// --------------------------------------------------------
// [EN] Interrupt Service Routines (ISR)
// [CN] 中断服务函数：越短越好，绝对不能有 delay 或 Serial.print
// --------------------------------------------------------
void ISR_Encoder_L() {
    // 当 A 相发生上升沿时，读取 B 相的电平来判断方向
    if (digitalRead(PIN_ENC_L_B) == LOW) {
        encoder_count_L++;
    } else {
        encoder_count_L--;
    }
}

void ISR_Encoder_R() {
    if (digitalRead(PIN_ENC_R_B) == LOW) {
        encoder_count_R--; // 注意：由于电机是对称安装的，左右轮的极性可能相反，后续联调时可能需要把 ++ 和 -- 互换
    } else {
        encoder_count_R++;
    }
}

// --------------------------------------------------------
// [EN] Initialization
// [CN] 中断初始化
// --------------------------------------------------------
void Encoder_Init() {
    // 确保在 hw_config.cpp 中，引脚已经被配置为 INPUT_PULLUP
    
    // 绑定外部中断：指定引脚，指定中断服务函数，指定触发条件 (RISING = 上升沿触发)
    // 必须使用 digitalPinToInterrupt 将物理引脚号转为底层中断号
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_L_A), ISR_Encoder_L, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_R_A), ISR_Encoder_R, RISING);
}

// --------------------------------------------------------
// [EN] Speed Readers (Atomic Access)
// [CN] 速度读取函数 (包含原子级防撕裂保护)
// --------------------------------------------------------
int16_t Get_Encoder_Speed_L() {
    int16_t speed;

    // [CN] 强行关闭全局中断。防止在我们复制 32 位数据的中途，突然来了一个脉冲把数据改了，导致数据撕裂
    // [EN] Critical Section: Disable interrupts to prevent data tearing when reading 32-bit encoder count
    noInterrupts(); 
    speed = encoder_count_L; // 提取速度 (5ms 内积累的脉冲数)
    encoder_count_L = 0;     // 计数器清零，为下一个 5ms 周期做准备
    interrupts();            // 恢复全局中断
    
    return speed;
}

int16_t Get_Encoder_Speed_R() {
    int16_t speed;
    noInterrupts(); 
    speed = encoder_count_R;
    encoder_count_R = 0;
    interrupts();
    return speed;
}

int32_t Get_Encoder_Count_L() {
    
    noInterrupts(); 
    count_L += encoder_count_L;
    encoder_count_L = 0; // Clear the encoder count after reading
    interrupts();
    return count_L;
}

int32_t Get_Encoder_Count_R() {

    noInterrupts(); 
    count_R += encoder_count_R;
    encoder_count_R = 0; // Clear the encoder count after reading
    interrupts();
    return count_R;
}