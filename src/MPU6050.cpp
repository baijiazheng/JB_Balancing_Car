#include"MPU6050.h"
#include<Wire.h>
#include <math.h> // [EN] Need math library for atan2() / [CN] 需要数学库调用 atan2() 反三角函数

//[EN]Actually define and allocate memory for these global variables
//[CN]真正定义并分配内存给这些全局变量
int16_t accelX, accelY, accelZ;
int16_t gyroX, gyroY, gyroZ;
int16_t tempRaw;
float pitchAngle = 0.0; // 最终融合的倾角
float gyroRateX = 0.0;  // 真实的角速度(度/秒)，后面 PID 的微分项要用到

//[EN]Implementation of the specific initialization function
//[CN]具体的初始化函数实现
void MPU6050_Init() {
  Wire.begin();
  Wire.setClock(400000); // Set I2C clock to 400 kHz for faster communication
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(PWR_MGMT_1);
  Wire.write(0x00); // Wake up the MPU6050
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(CONFIG);
  Wire.write(0x03); // Low-pass filtering at 44 Hz
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(GYRO_CONFIG);
  Wire.write(0x18); // Set gyro full scale to ±2000 °/s
  Wire.endTransmission(true);
  
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_CONFIG);
  Wire.write(0x00); // Set accel full scale to ±2 g
  Wire.endTransmission(true);
}

//[EN]Implementation of the function to read raw data from the sensor
//[CN]从传感器读取原始数据的函数实现
void MPU6050_Read_RAW() {
  Wire.beginTransmission(MPU6050_ADDR);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false); // Restart for read
  
  Wire.requestFrom(MPU6050_ADDR, 14,true); // Request 14 bytes of data
  
  if (Wire.available() >= 14) {
    accelX = Wire.read() << 8 | Wire.read(); // Combine high and low bytes
    accelY = Wire.read() << 8 | Wire.read();
    accelZ = Wire.read() << 8 | Wire.read();
    tempRaw = Wire.read() << 8 | Wire.read();
    gyroX = Wire.read() << 8 | Wire.read();
    gyroY = Wire.read() << 8 | Wire.read();
    gyroZ = Wire.read() << 8 | Wire.read();
  }
}

// [EN] Complementary Filter Implementation
// [CN] 互补滤波算法实现
void MPU6050_Calculate_Angle(float dt) {
  // 1. [EN] Calculate angle from Accelerometer using atan2 (Output in Degrees)
  // 1. [CN] 利用反三角函数计算加速度计的倾角 (弧度转为角度: * 57.2958)
  // 注意：具体用 accY 还是 accX，取决于你模块安装的方向，这里假设绕 X 轴旋转
  float accAngle = atan2(accelY, accelZ) * 57.2958;

  // 2. [EN] Convert raw Gyro data to deg/s (16.4 LSB/(deg/s) for ±2000 deg/s)
  // 2. [CN] 将陀螺仪原始数据转换为真实的角速度 (除以 16.4)
  gyroRateX = gyroX / 16.4;

  // 3. [EN] Apply Complementary Filter
  // 3. [CN] 应用互补滤波核心公式
  float alpha = 0.98;
  pitchAngle = alpha * (pitchAngle + gyroRateX * dt) + (1.0 - alpha) * accAngle;
}