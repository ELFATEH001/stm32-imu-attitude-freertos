// firmware/app/app.cpp
#include "app.hpp"
#include "I2CBus.hpp"
#include "main.h"
#include "mpu6500.hpp"

void appMain() {
  I2CBus bus(hi2c1);
  Mpu6500 mpu(bus);

  uint8_t whoAmI = 0;
  bool ok = mpu.whoAmI(whoAmI);
  (void)ok;
  while (true) {
    /* code */
  }
}
