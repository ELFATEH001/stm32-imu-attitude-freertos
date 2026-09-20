// firmware/drivers/Mpu6500.cpp
#include "mpu6500.hpp"
#include <array>

namespace {
constexpr uint8_t REG_WHO_AM_I = 0x75;
constexpr uint8_t REG_PWR_MGMT_1 = 0x6B;
constexpr uint8_t REG_SMPLRT_DIV = 0x19;
constexpr uint8_t REG_GYRO_CONFIG = 0x1B;
constexpr uint8_t REG_ACCEL_CONFIG = 0x1C;
constexpr uint8_t REG_ACCEL_XOUT_H = 0x3B;
constexpr uint8_t WHO_AM_I_EXPECTED = 0x70;

float gyroScaleFor(Mpu6500GyroscopeRange range) {
  switch (range) {
  case Mpu6500GyroscopeRange::Dps250:
    return 131.0f;
  case Mpu6500GyroscopeRange::Dps500:
    return 65.5f;
  case Mpu6500GyroscopeRange::Dps1000:
    return 32.8f;
  case Mpu6500GyroscopeRange::Dps2000:
    return 16.4f;
  }
  return 131.0f;
}

float accelScaleFor(Mpu6500AccelerometerRange range) {
  switch (range) {
  case Mpu6500AccelerometerRange::G2:
    return 16384.0f;
  case Mpu6500AccelerometerRange::G4:
    return 8192.0f;
  case Mpu6500AccelerometerRange::G8:
    return 4096.0f;
  case Mpu6500AccelerometerRange::G16:
    return 2048.0f;
  }
  return 16384.0f;
}
} // namespace

bool Mpu6500::whoAmI(uint8_t &outId) {
  return bus_.readRegister(static_cast<uint8_t>(devAddr_), REG_WHO_AM_I, &outId,
                           1);
}

bool Mpu6500::init(Mpu6500GyroscopeRange gyroRange,
                   Mpu6500AccelerometerRange accelRange) {
  uint8_t id = 0;
  if (!whoAmI(id) || id != WHO_AM_I_EXPECTED) {
    return false;
  }
  if (!bus_.writeRegister(static_cast<uint8_t>(devAddr_), REG_PWR_MGMT_1,
                          0x01)) {
    return false;
  }
  bus_.halDelay(50);

  if (!bus_.writeRegister(static_cast<uint8_t>(devAddr_), REG_SMPLRT_DIV,
                          0x00)) {
    return false;
  }

  uint8_t gyroConf = static_cast<uint8_t>(gyroRange)
                     << 3; // FS_SEL lives in bits [4:3]
  if (!bus_.writeRegister(static_cast<uint8_t>(devAddr_), REG_GYRO_CONFIG,
                          gyroConf)) {
    return false;
  }

  uint8_t accelConf = static_cast<uint8_t>(accelRange)
                      << 3; // AFS_SEL lives in bits [4:3]
  if (!bus_.writeRegister(static_cast<uint8_t>(devAddr_), REG_ACCEL_CONFIG,
                          accelConf)) {
    return false;
  }

  gyroScale_ = gyroScaleFor(gyroRange);
  accelScale_ = accelScaleFor(accelRange);
  return true;
}

ImuSample Mpu6500::read() {
  std::array<uint8_t, 14> raw = {0};
  bus_.readRegister(static_cast<uint8_t>(devAddr_), REG_ACCEL_XOUT_H,
                    raw.data(), 14);

  RawImuSample rs;
  // acc
  rs.ax = static_cast<int16_t>((raw[0] << 8) | raw[1]);
  rs.ay = static_cast<int16_t>((raw[2] << 8) | raw[3]);
  rs.az = static_cast<int16_t>((raw[4] << 8) | raw[5]);
  // temp
  // raw.at(6), raw.at(7)
  // gyro
  rs.gx = static_cast<int16_t>((raw[8] << 8) | raw[9]);
  rs.gy = static_cast<int16_t>((raw[10] << 8) | raw[11]);
  rs.gz = static_cast<int16_t>((raw[12] << 8) | raw[13]);

  ImuSample s;
  s.accelG = {rs.ax / accelScale_, rs.ay / accelScale_, rs.az / accelScale_};
  s.gyroDps = {rs.gx / gyroScale_ - gyroBiasDps_[0],
               rs.gy / gyroScale_ - gyroBiasDps_[1],
               rs.gz / gyroScale_ - gyroBiasDps_[2]};
  return s;
}

void Mpu6500::calibrateGyroBias(uint16_t samples) {
  float sumX = 0.0f, sumY = 0.0f, sumZ = 0.0f;
  for (uint16_t i = 0; i < samples; ++i) {
    ImuSample s = read();
    sumX += s.gyroDps[0];
    sumY += s.gyroDps[1];
    sumZ += s.gyroDps[2];
    bus_.halDelay(2);
  }
  gyroBiasDps_ = {sumX / samples, sumY / samples, sumZ / samples};
}