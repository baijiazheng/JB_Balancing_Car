#include "Motor.h"
#include "HW_CONFIG.h"
#include "Control.h"

// [EN] Write PWM value to motor pin, considering active low/high configuration
// [CN] 向电机引脚写入 PWM 值，考虑高低电平配置
void Motor_WritePWM(uint8_t pin, uint8_t duty)
{
    if (PWM_ACTIVE_LOW)
        analogWrite(pin, 255 - duty);
    else
        analogWrite(pin, duty);
}

// [EN] Set Motor speeds
// [CN] 设置电机速度，正数正转，负数反转，0为刹车
void Set_Motor(int32_t speed_L, int32_t speed_R) {
    // 1. 绝对安全限幅 (防整型溢出引发的极性反转)
    speed_L = constrain(speed_L, -Motor_MAX_PWM, Motor_MAX_PWM);
    speed_R = constrain(speed_R, -Motor_MAX_PWM, Motor_MAX_PWM);

    // 2. 驱动左轮 (Left Motor)
    if (speed_L == 0) {
        digitalWrite(PIN_MOTOR_L_IN1, LOW);
        digitalWrite(PIN_MOTOR_L_IN2, LOW);
        Motor_WritePWM(PIN_MOTOR_L_PWM, 0);
    } else if (speed_L > 0) {
        digitalWrite(PIN_MOTOR_L_IN1, HIGH);
        digitalWrite(PIN_MOTOR_L_IN2, LOW);
        Motor_WritePWM(PIN_MOTOR_L_PWM, speed_L);
    } else {
        digitalWrite(PIN_MOTOR_L_IN1, LOW);
        digitalWrite(PIN_MOTOR_L_IN2, HIGH);
        Motor_WritePWM(PIN_MOTOR_L_PWM, -speed_L); // PWM 占空比必须是正数
    }

    // 3. 驱动右轮 (Right Motor)
    if (speed_R == 0) {
        digitalWrite(PIN_MOTOR_R_IN1, LOW);
        digitalWrite(PIN_MOTOR_R_IN2, LOW);
        Motor_WritePWM(PIN_MOTOR_R_PWM, 0);
    } else if (speed_R > 0) {
        digitalWrite(PIN_MOTOR_R_IN1, HIGH);
        digitalWrite(PIN_MOTOR_R_IN2, LOW);
        Motor_WritePWM(PIN_MOTOR_R_PWM, speed_R);
    } else {
        digitalWrite(PIN_MOTOR_R_IN1, LOW);
        digitalWrite(PIN_MOTOR_R_IN2, HIGH);
        Motor_WritePWM(PIN_MOTOR_R_PWM, -speed_R);
    }
}

void Drive_Motor(float pitchAngle,int16_t motor_out) {
    int16_t filtered_PWM = Motor_Output_Filter(motor_out, pitchAngle);

    Set_Motor(filtered_PWM, filtered_PWM);
}