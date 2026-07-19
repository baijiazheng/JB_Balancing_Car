#include "Control.h"
#include "Motor.h"
// [EN] Initialize parameters (These values need to be tuned on the real hardware)
// [CN] 初始化参数 (这些极性及具体数值，必须在真机上一点点调出来)
float Kp_Upright = 30;  // 比例系数 (力度)
float Kd_Upright = 1.5;   // 微分系数 (阻尼)
float Target_Angle = 2.0; // 假设重心零点是 2.0 度

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