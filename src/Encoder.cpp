#include "HW_CONFIG.h"
#include "Encoder.h"
#include "Bluetooth.h"

volatile int32_t encoder_count_L = 0;
volatile int32_t encoder_count_R = 0;
volatile int32_t encoder_delta_L = 0;
volatile int32_t encoder_delta_R = 0;
volatile int32_t encoder_tick_L = 0;
volatile int32_t encoder_tick_R = 0;
// [EN] Static counter hidden from global scope (Encapsulation)
// [CN] 隐藏在局部作用域的静态计数器，实现面向对象级别的封装，防止被外部意外篡改
static uint8_t speed_counter = 0;
int32_t speed_car = 0; // [EN] Average speed of the car / [CN] 小车的平均速度

// --------------------------------------------------------
// [EN] Interrupt Service Routines (ISR)
// [CN] 中断服务函数：越短越好，绝对不能有 delay 或 Serial.print
// --------------------------------------------------------
void ISR_Encoder_L() {
    // 当 A 相发生上升沿时，读取 B 相的电平来判断方向
    if (digitalRead(PIN_ENC_L_B) == LOW) {
        encoder_count_L++;
        encoder_delta_L++;
    } else {
        encoder_count_L--;
        encoder_delta_L--;
    }
}

void ISR_Encoder_R() {
    if (digitalRead(PIN_ENC_R_B) == LOW) {
        encoder_count_R--; // 注意：由于电机是对称安装的，左右轮的极性可能相反，后续联调时可能需要把 ++ 和 -- 互换
        encoder_delta_R--;
    } else {
        encoder_count_R++;
        encoder_delta_R++;
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
int32_t Get_Encoder_Speed_L() {
    int32_t speed;

    // [CN] 强行关闭全局中断。防止在我们复制 32 位数据的中途，突然来了一个脉冲把数据改了，导致数据撕裂
    // [EN] Critical Section: Disable interrupts to prevent data tearing when reading 32-bit encoder count
    noInterrupts(); 
    speed = encoder_delta_L; // 提取速度 (积累的脉冲数)
    speed *= ENC_Hz; // 转换为每秒脉冲数 (Hz)
    encoder_delta_L = 0;     // 计数器清零，为下一个周期做准备
    interrupts();            // 恢复全局中断
    
    return speed;
}

int32_t Get_Encoder_Speed_R() {
    int32_t speed;
    
    noInterrupts(); 
    speed = encoder_delta_R;
    speed *= ENC_Hz; 
    encoder_delta_R = 0;
    interrupts();
    return speed;
}

void Get_Speed (){
    speed_counter++;
    int16_t speed_counter_limit = (200 / ENC_Hz); // [EN] Calculate speed every 50ms / [CN] 每50ms计算一次速度
    if (speed_counter >= speed_counter_limit) {
        speed_counter = 0; 
        int32_t speed_L = Get_Encoder_Speed_L();
        int32_t speed_R = Get_Encoder_Speed_R();
        speed_car = (speed_L + speed_R) / 2; 
        BT_SERIAL.println(speed_car);
    }
}

int32_t Get_Encoder_Tick_L() {
    
    noInterrupts(); 
    encoder_tick_L += encoder_count_L;
    encoder_count_L = 0; // Clear the encoder count after reading
    interrupts();
    return encoder_tick_L;
}

int32_t Get_Encoder_Tick_R() {

    noInterrupts(); 
    encoder_tick_R += encoder_count_R;
    encoder_count_R = 0; // Clear the encoder count after reading
    interrupts();
    return encoder_tick_R;
}