// firmware/app/ComplementaryFilter.cpp
#include "ComplementaryFilter.hpp"
#include <array>
#include <cmath>

namespace {
constexpr float RAD_TO_DEG = 57.29577951308232f;
}

ComplementaryFilter::ComplementaryFilter(float alpha, float dtSeconds)
    : alpha_(alpha), dtSeconds_(dtSeconds) {}

void ComplementaryFilter::update(const std::array<float, 3> &accelG,
                                 const std::array<float, 3> &gyroDps) {

  // Accel
  float accelRollDeg = std::atan2(accelG[1], accelG[2]) * RAD_TO_DEG;
  float accelPitchDeg =
      std::atan2(-accelG[0],
                 std::sqrt(accelG[1] * accelG[1] + accelG[2] * accelG[2])) *
      RAD_TO_DEG;

  // Gyro
  float gyroRollDeg = rollDeg_ + gyroDps[0] * dtSeconds_;
  float gyroPitchDeg = pitchDeg_ + gyroDps[1] * dtSeconds_;

  // filter
  rollDeg_ = alpha_ * gyroRollDeg + (1.0f - alpha_) * accelRollDeg;
  pitchDeg_ = alpha_ * gyroPitchDeg + (1.0f - alpha_) * accelPitchDeg;
}

float ComplementaryFilter::roll() const { return rollDeg_; }
float ComplementaryFilter::pitch() const { return pitchDeg_; }