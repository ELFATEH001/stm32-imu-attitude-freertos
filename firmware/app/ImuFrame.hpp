// firmeware/app/ImuFrame.hpp
#pragma once
#include "ImuMsg.h"

static_assert(sizeof(ImuMsg) == 32,
              "ImuMsg layout changed — update receiver parser");

uint8_t computeChecksum(const ImuMsg &msg) {
  const uint8_t *bytes = reinterpret_cast<const uint8_t *>(&msg);
  uint8_t sum = 0;
  for (size_t i = 0; i < sizeof(ImuMsg); ++i) {
    sum ^= bytes[i];
  }
  return sum;
}

ImuFrame buildFrame(const ImuMsg &msg) {
  ImuFrame frame;
  frame.sync1 = IMU_FRAME_SYNC1;
  frame.sync2 = IMU_FRAME_SYNC2;
  frame.payload = msg;
  frame.checksum = computeChecksum(msg);
  return frame;
}
static_assert(sizeof(ImuFrame) == 35,
              "ImuFrame layout changed — update receiver parser");