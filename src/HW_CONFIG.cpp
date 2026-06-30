#include "HW_CONFIG.h"

// [EN] Initialize all pins to their SAFE STATES immediately upon boot
// [CN] 在系统启动时，立刻将所有引脚初始化为“绝对安全状态”
void Hardware_Init() {
    // -----------------------------------
    // 1. Configure Pin Directions (输入/输出)
    // -----------------------------------
    pinMode(PIN_MOTOR_L_PWM, OUTPUT);
    pinMode(PIN_MOTOR_L_IN1, OUTPUT);
    pinMode(PIN_MOTOR_L_IN2, OUTPUT);
    
    pinMode(PIN_MOTOR_R_PWM, OUTPUT);
    pinMode(PIN_MOTOR_R_IN1, OUTPUT);
    pinMode(PIN_MOTOR_R_IN2, OUTPUT);
    

    // 编码器引脚设为输入，并开启内部上拉电阻，防止悬空带来高频噪声干扰
    pinMode(PIN_ENC_L_A, INPUT_PULLUP);
    pinMode(PIN_ENC_L_B, INPUT_PULLUP);
    pinMode(PIN_ENC_R_A, INPUT_PULLUP);
    pinMode(PIN_ENC_R_B, INPUT_PULLUP);

    // [致命防御] 锁死电机 PWM 为 0，方向为刹车状态，防止上电疯跑
    analogWrite(PIN_MOTOR_L_PWM, 0);
    digitalWrite(PIN_MOTOR_L_IN1, LOW);
    digitalWrite(PIN_MOTOR_L_IN2, LOW);
    
    analogWrite(PIN_MOTOR_R_PWM, 0);
    digitalWrite(PIN_MOTOR_R_IN1, LOW);
    digitalWrite(PIN_MOTOR_R_IN2, LOW);
}