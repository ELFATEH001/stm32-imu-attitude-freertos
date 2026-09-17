// firmware/hal/I2CBus.hpp
#pragma once
#include "stm32f4xx_hal.h"
#include <cstddef>
#include <cstdint>

class I2CBus {
public:
  explicit I2CBus(I2C_HandleTypeDef &handle) : handle_(handle) {}
  bool readRegister(uint8_t devAddr, uint8_t regAddr, uint8_t *data,
                    uint16_t len);
  bool writeRegister(uint8_t devAddr, uint8_t regAddr, const uint8_t *data,
                     uint16_t len);
  bool isDeviceReady(uint8_t devAddr, uint32_t trails = 3);

private:
  I2C_HandleTypeDef &handle_;
};
