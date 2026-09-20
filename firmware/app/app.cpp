// firmware/app/app.cpp
#include "app.hpp"
#include "I2CBus.hpp"
#include "main.h"
#include "mpu6500.hpp"

void appMain() {
  I2CBus bus(hi2c1);
  Mpu6500 mpu(bus);

  // uint8_t whoAmI = 0;
  // bool ok = mpu.whoAmI(whoAmI);
  bool init =
      mpu.init(Mpu6500GyroscopeRange::Dps250, Mpu6500AccelerometerRange::G2);

  // (void)ok;
  if (!init) {
    while (true) {
      /* breakpoint: init failed — check wiring/address */
    };
  }
  mpu.calibrateGyroBias();
  while (true) {
    ImuSample data = mpu.read();
    // breakpoint here: inspect data.accelG / data.gyroDps
  }
}
