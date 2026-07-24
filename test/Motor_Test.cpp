#include "Motor_Test.h"
#include "Motor.h"
#include "HW_CONFIG.h"

void Motor_Test_Init() {
    Hardware_Init(); // [EN]Initialize hardware peripherals/[CH]初始化硬件外设
}

// [EN] Motor Diagnostic Sweep Routine
// [CN] 电机扫频压测函数
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
