// firmware/app/ImuMsg.h
#pragma once
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
  float accelG[3];
  float gyroDps[3];
} ImuMsg;

#define IMU_FRAME_SYNC1 0xAAu
#define IMU_FRAME_SYNC2 0x55u

typedef struct __attribute__((packed)) {
  uint8_t sync1;
  uint8_t sync2;
  ImuMsg payload;
  uint8_t checksum;
} ImuFrame;

#ifdef __cplusplus
}
#endif