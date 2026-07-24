#include "Bluetooth.h"

// [EN] Static counter hidden from global scope (Encapsulation)
// [CN] 隐藏在局部作用域的静态计数器，实现面向对象级别的封装，防止被外部意外篡改
static uint8_t tele_counter = 0;

void Telemetry_Init() {
    // 开启硬件串口
    BT_SERIAL.begin(BT_BAUDRATE);
    BT_SERIAL.println("");
    BT_SERIAL.println("===============================");
    BT_SERIAL.println("[SYS] BT Telemetry Activated!");
    BT_SERIAL.println("===============================");
}

// --------------------------------------------------------
// [EN] Core Essence: Downsampled Transmission (10Hz)
// [CN] 核心本质：降频发送架构。将高频的控制循环与低频的通信循环物理剥离。
// --------------------------------------------------------
void Telemetry_Send_100ms(float pitch, float gyro_rate) {
    tele_counter++;
    
    // 阈值 20 * 调用周期 5ms = 100ms 物理周期
    if (tele_counter >= 20) {
        tele_counter = 0; // 倒空计数器
        
        // 发送极其紧凑的数据帧，节约 UART 缓冲区
        // 格式: P:1.2,W:0\n
        BT_SERIAL.print("P:");
        BT_SERIAL.print(pitch, 1); // 仅保留1位小数
        BT_SERIAL.print(",G:");
        BT_SERIAL.println(gyro_rate, 1); // 仅保留1位小数
    }
}

// --------------------------------------------------------
// [EN] Core Essence: Non-blocking Reception
// [CN] 核心本质：绝对零阻塞接收。只要缓冲区没数据，立刻返回空字符退出，绝不空转等待。
// --------------------------------------------------------
char Telemetry_Receive_Cmd() {
    if (BT_SERIAL.available() > 0) {
        return BT_SERIAL.read(); // 读取单个字符命令 (如 'F', 'B', 'S')
    }
    return '\0'; // 没有收到数据时返回空字符 (NULL)
}