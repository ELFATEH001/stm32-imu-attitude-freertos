// firmware/drivers/Mpu6500.cpp
#include "mpu6500.hpp"

bool Mpu6500::whoAmI(uint8_t &outId) {
  return bus_.readRegister(devAddr_, 0x75, &outId, 1);
}