// firmware/drivers/Mpu6500.hpp
#pragma once
#include "I2CBus.hpp"
#include <array>

enum class Mpu6500Address : uint8_t {
  AdoLow = 0x68,
  AdoHigh = 0x69,
};

enum class Mpu6500GyroscopeRange : uint8_t {
  Dps250 = 0x00,  // ±250 dps
  Dps500 = 0x01,  // ±500 dps
  Dps1000 = 0x02, // ±1000 dps
  Dps2000 = 0x03, // ±2000 dps
};

enum class Mpu6500AccelerometerRange : uint8_t {
  G2 = 0x00,  // ±2 g
  G4 = 0x01,  // ±4 g
  G8 = 0x02,  // ±8 g
  G16 = 0x03, // ±16 g
};

struct RawImuSample {
  int16_t ax, ay, az, gx, gy, gz;
};

struct ImuSample {
  std::array<float, 3> accelG;
  std::array<float, 3> gyroDps;
};

class Mpu6500 {
public:
  explicit Mpu6500(I2CBus &bus, Mpu6500Address devAddr = Mpu6500Address::AdoLow)
      : bus_(bus), devAddr_(devAddr) {}
  bool
  init(Mpu6500GyroscopeRange gyroRange = Mpu6500GyroscopeRange::Dps250,
       Mpu6500AccelerometerRange accelRange = Mpu6500AccelerometerRange::G2);
  bool whoAmI(uint8_t &outId);
  ImuSample read();
  void calibrateGyroBias(uint16_t samples = 500);

private:
  I2CBus &bus_;
  Mpu6500Address devAddr_;
  float accelScale_, gyroScale_;
  std::array<float, 3> gyroBiasDps_ = {0.0f, 0.0f, 0.0f};
};
