#include "Control.h"
#include "Motor.h"
#include "Encoder.h"

// [EN] Initialize parameters (These values need to be tuned on the real hardware)
// [CN] 初始化参数 (这些极性及具体数值，必须在真机上一点点调出来)
float Kp_Upright = 11;  // 比例系数 (力度)
float Kd_Upright = 1.5;   // 微分系数 (阻尼)
float Kp_velocity = -0.01;
float Ki_velocity = 0;
float Mec_Zero = 6.0;
float Target_Angle = 6.0; // 假设重心零点
float Target_Speed = 0;

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

    if(abs(pitchAngle-Target_Angle) < 15.0) {
        limit = 2; // [EN] If the angle is small, allow a smaller change / [CN] 如果角度小，允许较小的变化量
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
        limit = 20; // [EN] If the angle is moderate, allow a larger change / [CN] 如果角度适中，允许较大的变化量
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

// [EN] Velocity Loop PID (PI Control)
// [CN] 速度外环 PI 控制算法
void Get_Target_Angle (){
    static int8_t counter = 0;
    static float speed_error_sum = 0;
    static int32_t last_speed_error = 0;
    ++counter;
    
    if (counter >= 3){
        // 1. 计算当前速度偏差
        int32_t speed_error = speed_car - Target_Speed;
        
        // 2. 速度平滑滤波（一阶低通滤波，消除编码器采样高频抖动）
        // 0.7 和 0.3 为滤波系数，根据实际响应微调
        float filtered_error = speed_error * 0.3f + last_speed_error * 0.7f;
        last_speed_error = filtered_error;
        
        // 3. 累加积分 (I项)
        speed_error_sum += filtered_error;
        
        // 4. 积分限幅 (Anti-Windup)，防止饱和发疯
        speed_error_sum = constrain(speed_error_sum,-SPEED_I_LIMIT,SPEED_I_LIMIT);
        
        // 5. 计算速度环对角度的修正量
        // 注意：这里的 Kp 和 Ki 必须非常小，因为输出量直接加到角度上！
        float Angle_Offset = (filtered_error * Kp_velocity) + (speed_error_sum * Ki_velocity);
        
        // 6. 角度修正量限幅
        Angle_Offset = constrain (Angle_Offset,-TARGET_ANGLE_LIMIT,TARGET_ANGLE_LIMIT);

        // 7. 最终目标角度 = 机械零点 + 速度环输出的角度偏移量
        Target_Angle = Mec_Zero + Angle_Offset;

        counter = 0;
    }

}