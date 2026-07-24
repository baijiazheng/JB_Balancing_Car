#include <Arduino.h>
#include "MPU6050.h"
#include "Control.h"
#include "Bluetooth.h"
#include "Timer1.h"
#include "HW_CONFIG.h"
#include "Motor.h"
#include "Encoder.h"
#include "System_Mode.h"
#include "Balance_Mode.h"
#include "Encoder_Test.h"
#include "Motor_Test.h"
#include "Bluetooth_Config.h"

// ================= 主系统入口 =================
void setup() {
    #if SYSTEM_MODE == 1
    Balance_Mode_Init();
    #elif SYSTEM_MODE == 2
    Encoder_Test_Init();
    #elif SYSTEM_MODE == 3
    Motor_Test_Init();
    #elif SYSTEM_MODE == 4
    Telemetry_CONFIG(); 
    #endif
}

// ================== 主循环 ==================
    void loop() {
    #if SYSTEM_MODE == 1
    Balance_Mode_Loop();
    #elif SYSTEM_MODE == 2
    Encoder_Test();
    #elif SYSTEM_MODE == 3
    Motor_Diagnostic_Test();
    #endif
}