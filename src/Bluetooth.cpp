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
void Telemetry_Send_100ms(float pitch, int16_t motor_pwm) {
    tele_counter++;
    
    // 阈值 20 * 调用周期 5ms = 100ms 物理周期
    if (tele_counter >= 20) {
        tele_counter = 0; // 倒空计数器
        
        // 发送极其紧凑的数据帧，节约 UART 缓冲区
        // 格式: P:1.2,W:0\n
        BT_SERIAL.print("P:");
        BT_SERIAL.print(pitch, 1); // 仅保留1位小数
        BT_SERIAL.print(",W:");
        BT_SERIAL.println(motor_pwm); // println 自动携带 \r\n 换行符作为帧结束标志
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

void Telemetry_CONFIG() {
    //[EN]if there are any errors here, please visit the Bluetooth_Config_test in my GitHub repository for a more detailed configuration guide.
    //[CN]如果这里有任何错误，请访问我的 GitHub 仓库中的 Bluetooth_Config_test 获取更详细的配置指南。
    // 这里可以根据需要配置蓝牙模块的波特率,只运行一次即可,如果不需要配置可以留空
    Serial.begin(115200); // 设置电脑串口波特率为 115200
    BT_SERIAL.begin(9600); // 设置波特率为 9600, 这是大多数蓝牙模块的默认波特率
    delay(1000); // 等待蓝牙模块启动
    BT_SERIAL.print("AT"); // 设置波特率为 115200
    delay(1000); // 等待设置生效
    while (BT_SERIAL.available()) {
        Serial.write(BT_SERIAL.read()); // 将蓝牙模块的响应输出到电脑串口监视器
    }
    BT_SERIAL.print("AT+BAUD8"); // 设置波特率为 115200
    delay(1000); // 等待设置生效
    while (BT_SERIAL.available()) {
        Serial.write(BT_SERIAL.read()); // 将蓝牙模块的响应输出到电脑串口监视器
    }
    Serial.println("Bluetooth module configured to 115200 baud.");
}
