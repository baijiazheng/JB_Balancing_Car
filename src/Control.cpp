#include "Control.h"
#include "Motor.h"
#include "Encoder.h"

// [EN] Initialize parameters (These values need to be tuned on the real hardware)
// [CN] 初始化参数 (这些极性及具体数值，必须在真机上一点点调出来)
float Kp_Upright = 11;  // 比例系数 (力度)
float Kd_Upright = 1.5;   // 微分系数 (阻尼)
float Target_Angle = 8.8; // 假设重心零点是 8.8 度

// [EN] Upright PD Control Implementation
// [CN] 直立环 PD 控制实现
int16_t Upright_PD_Control(float pitch, float gyro_rate) {
    float upright_pwm = 0;

    // [EN] Calculate Angle Error
    // [CN] 计算角度偏差 (当前角度 - 目标重心角度)
    float angle_error = pitch - Target_Angle;

    // [EN] Calculate the PD control output
    // [CN] 计算 PD 控制输出
    upright_pwm = Kp_Upright * angle_error + Kd_Upright * gyro_rate;

    // 3. [EN] Cast float to 16-bit integer
    // 3. [CN] 安全地将浮点数截断为 16 位整型
    float final_pwm = static_cast<float>(upright_pwm);

    // 4. [EN] Hardware Limit:Limit output to Motor_MAX_PWM
    // 4. [CN] 物理限幅:限制输出在 Motor_MAX_PWM 范围内
    final_pwm = constrain(final_pwm,-Motor_MAX_PWM,Motor_MAX_PWM);

    return (int16_t)final_pwm ;
}

int16_t Motor_Output_Filter(int16_t target_PWM,float pitchAngle) {
    int16_t limit = 0; // [EN] Define the maximum change per 5ms / [CN] 定义每5ms的最大变化量
    static int16_t previous_output_PWM = 0; // [EN] Store the previous speed / [CN] 存储上一次的速度值
    
    if (abs(pitchAngle) > 45.0) {
        return previous_output_PWM=0; // [EN] If the angle is too large, stop the motors / [CN] 如果角度过大，停止电机
    }
    if(abs(target_PWM)<20){
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

//---velocity loop filter---

    