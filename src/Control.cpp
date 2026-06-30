#include "Control.h"

// [EN] Initialize parameters (These values need to be tuned on the real hardware)
// [CN] 初始化参数 (这些极性及具体数值，必须在真机上一点点调出来)
float Kp_Upright = 30.0;  // 比例系数 (力度)
float Kd_Upright = 0.8;   // 微分系数 (阻尼)
float Target_Angle = 0.0; // 假设重心零点是 0 度

// [EN] Upright PD Control Implementation
// [CN] 直立环 PD 控制实现
int16_t Upright_PD_Control(float pitch, float gyro_rate) {
    int16_t upright_pwm = 0;

    // [EN] Calculate Angle Error
    // [CN] 计算角度偏差 (当前角度 - 目标重心角度)
    float angle_error = pitch - Target_Angle;

    // [EN] Calculate the PD control output
    // [CN] 计算 PD 控制输出
    upright_pwm = Kp_Upright * angle_error + Kd_Upright * gyro_rate;

    // 3. [EN] Cast float to 16-bit integer
    // 3. [CN] 安全地将浮点数截断为 16 位整型
    int16_t final_pwm = static_cast<int16_t>(upright_pwm);

    // 4. [EN] Hardware Limit: Match Timer 3 TOP value (400)
    // 4. [CN] 物理限幅：必须匹配 Timer 3 寄存器设定的 TOP 值 (400)
    if (final_pwm > 400) {
        final_pwm = 400;
    } else if (final_pwm < -400) {
        final_pwm = -400;
    }

    return final_pwm;
}