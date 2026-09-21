// firmware/app/ComplementaryFilter.hpp

#pragma once
#include <array>

class ComplementaryFilter {
public:
  explicit ComplementaryFilter(float alpha, float dtSeconds);
  void update(const std::array<float, 3> &accelG,
              const std::array<float, 3> &gyroDps);
  float roll() const;
  float pitch() const;

private:
  float alpha_;
  float dtSeconds_;
  float rollDeg_ = 0.0f;
  float pitchDeg_ = 0.0f;
};