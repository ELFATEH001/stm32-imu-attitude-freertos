// firmware/hal/I2CBus.cpp
#include "I2CBus.hpp"
#include <cstdint>
#include <cstddef>

bool I2CBus::readRegister(uint8_t devAddr, uint8_t regAddr, uint8_t *data, uint16_t len)
{
    return HAL_I2C_Mem_Read(
        &handle_, 
        devAddr << 1, 
        regAddr, 
        I2C_MEMADD_SIZE_8BIT, 
        data,
        len,
        HAL_MAX_DELAY) == HAL_OK;
}

bool I2CBus::writeRegister(uint8_t devAddr, uint8_t regAddr, const uint8_t *data, uint16_t len)
{
    return HAL_I2C_Mem_Write(
        &handle_, 
        devAddr << 1, 
        regAddr, 
        I2C_MEMADD_SIZE_8BIT, 
        const_cast<uint8_t*>(data),
        len,
        HAL_MAX_DELAY) == HAL_OK;
}

bool I2CBus::isDeviceReady(uint8_t devAddr, uint32_t trials)
{
    return HAL_I2C_IsDeviceReady(
        &handle_, 
        devAddr << 1, 
        trials, 
        HAL_MAX_DELAY) == HAL_OK;
}