// firmware/hal/I2CBus.h
#pragma once
#include <cstdint>
#include <cstddef>

class I2CBus
{
public:
    explicit I2CBus(void* handle) : handle_(handle) {}
    bool readRegister(uint8_t devAddr, uint8_t regAddr, uint8_t *data, size_t len);
    bool writeRegister(uint8_t devAddr, uint8_t regAddr, uint8_t *data);
    bool isDeviceReady(uint8_t defAddr, uint32_t trails =3);

private:
    void* handle_;
};

