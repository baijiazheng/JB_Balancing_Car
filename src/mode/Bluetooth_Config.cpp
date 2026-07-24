#include "Bluetooth_Config.h"
#include "Bluetooth.h"

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
