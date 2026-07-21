#include "Motor.h"
#include "HW_CONFIG.h"

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

// [EN] Motor Diagnostic Sweep Routine
// [CN] 电机扫频压测函数 (开机执行一次)
void Motor_Diagnostic_Test() {
    Set_Motor(100, 0);
    delay(300);
    Set_Motor(0,100);
    delay(300);
    Set_Motor(-100, 0);
    delay(300);
    Set_Motor(0,-100);
    delay(300);
    Set_Motor(100, 100);
    delay(300);
    Set_Motor(-100, -100);
    delay(300);
    Set_Motor(0,0);
}

int16_t Motor_Output_Filter(int16_t target_PWM,float pitchAngle) {
    int16_t limit = 0; // [EN] Define the maximum change per 5ms / [CN] 定义每5ms的最大变化量
    static int16_t previous_output_PWM = 0; // [EN] Store the previous speed / [CN] 存储上一次的速度值
    
    if (abs(pitchAngle) > 45.0) {
        return previous_output_PWM=0; // [EN] If the angle is too large, stop the motors / [CN] 如果角度过大，停止电机
    }
    if(abs(target_PWM)<10){
        target_PWM=0;
    }

    if(abs(pitchAngle) < 15.0) {
        limit = 1; // [EN] If the angle is small, allow a smaller change / [CN] 如果角度小，允许较小的变化量
        if (abs(target_PWM - previous_output_PWM) > limit) {
            if (target_PWM > previous_output_PWM) {
                previous_output_PWM += limit;
            } else {
                previous_output_PWM -= limit;
            }
        } else {
            previous_output_PWM = target_PWM;
        }
        return previous_output_PWM;
    }
    else if(previous_output_PWM * target_PWM < 0) { // [EN] If the direction changes, allow a larger change / [CN] 如果方向改变，允许较大的变化量
        limit = 10; // [EN] If the angle is moderate, allow a larger change / [CN] 如果角度适中，允许较大的变化量
        if (abs(target_PWM - previous_output_PWM) > limit) {
            if (target_PWM > previous_output_PWM) {
                previous_output_PWM += limit;
            } else {
                previous_output_PWM -= limit;
            }
        } else {
            previous_output_PWM = target_PWM;
        }
        return previous_output_PWM;
    } 
    else {
        previous_output_PWM = 0; // [EN] Reset previous output when angle is large / [CN] 当角度大时，重置上一次输出
        return target_PWM; // [EN] If the angle is large, no filtering / [CN] 如果角度大，不进行滤波
    }

}

void Drive_Motor(float pitchAngle,int16_t motor_out) {
    int16_t filtered_PWM = Motor_Output_Filter(motor_out, pitchAngle);

    Set_Motor(filtered_PWM, filtered_PWM);
}