// firmware/drivers/Mpu6500.hpp
#pragma once
#include "I2CBus.hpp"

class Mpu6500 {
public:
  explicit Mpu6500(I2CBus &bus, uint8_t devAddr = 0x68)
      : bus_(bus), devAddr_(devAddr) {}
  bool whoAmI(uint8_t &outId);

private:
  I2CBus &bus_;
  uint8_t devAddr_;
};
