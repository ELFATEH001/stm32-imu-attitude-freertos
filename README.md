# STM32 Attitude Estimation (FreeRTOS)

Real-time attitude estimation running on an STM32 microcontroller under FreeRTOS. The system reads IMU data from an MPU6500 sensor and fuses it into an orientation estimate (roll/pitch/yaw or quaternion) using a real-time task pipeline.

## Features

- Real-time sensor acquisition and attitude computation under FreeRTOS
- MPU6500 driver for accelerometer/gyroscope data
- C++ abstraction layer over the STM32 HAL
- Modular task-based architecture suited for extension (e.g. sensor fusion algorithm swaps, additional sensors)

## Repository Structure

The C++ application code lives in the `firmware/` folder, organized as follows:

firmware/
├── app → application logic (attitude estimation, tasks)
├── driver → MPU6500 sensor driver
└── hal → C++ wrapper over the STM32 HAL
