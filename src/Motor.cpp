#include "Motor.h"
#include "HW_CONFIG.h"

// [EN] Set Motor speeds (-255 to 255)
// [CN] 设置电机速度，正数正转，负数反转，0为刹车
void Set_Motor(int16_t speed_L, int16_t speed_R) {
    // 1. 绝对安全限幅 (防整型溢出引发的极性反转)
    speed_L = constrain(speed_L, -Motor_MAX_PWM, Motor_MAX_PWM);
    speed_R = constrain(speed_R, -Motor_MAX_PWM, Motor_MAX_PWM);

    // 2. 驱动左轮 (Left Motor)
    if (speed_L == 0) {
        digitalWrite(PIN_MOTOR_L_IN1, LOW);
        digitalWrite(PIN_MOTOR_L_IN2, LOW);
        analogWrite(PIN_MOTOR_L_PWM, 0);
    } else if (speed_L > 0) {
        digitalWrite(PIN_MOTOR_L_IN1, HIGH);
        digitalWrite(PIN_MOTOR_L_IN2, LOW);
        analogWrite(PIN_MOTOR_L_PWM, speed_L);
    } else {
        digitalWrite(PIN_MOTOR_L_IN1, LOW);
        digitalWrite(PIN_MOTOR_L_IN2, HIGH);
        analogWrite(PIN_MOTOR_L_PWM, -speed_L); // PWM 占空比必须是正数
    }

    // 3. 驱动右轮 (Right Motor)
    if (speed_R == 0) {
        digitalWrite(PIN_MOTOR_R_IN1, LOW);
        digitalWrite(PIN_MOTOR_R_IN2, LOW);
        analogWrite(PIN_MOTOR_R_PWM, 0);
    } else if (speed_R > 0) {
        digitalWrite(PIN_MOTOR_R_IN1, HIGH);
        digitalWrite(PIN_MOTOR_R_IN2, LOW);
        analogWrite(PIN_MOTOR_R_PWM, speed_R);
    } else {
        digitalWrite(PIN_MOTOR_R_IN1, LOW);
        digitalWrite(PIN_MOTOR_R_IN2, HIGH);
        analogWrite(PIN_MOTOR_R_PWM, -speed_R);
    }
}

// [EN] Motor Diagnostic Sweep Routine
// [CN] 电机扫频压测函数 (开机执行一次)
void Motor_Diagnostic_Test() {
    Set_Motor(100, 0);
    delay(3000);
    Set_Motor(0, 0);
}