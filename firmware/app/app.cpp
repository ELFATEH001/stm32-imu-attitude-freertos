// firmware/app/app.cpp
#include "app.hpp"
#include "ComplementaryFilter.hpp"
#include "FreeRTOS.h"
#include "I2CBus.hpp"
#include "ImuFrame.hpp"
#include "ImuMsg.h"
#include "cmsis_os.h"
#include "main.h"
#include "mpu6500.hpp"

// void appMain() {
//   I2CBus bus(hi2c1);
//   Mpu6500 mpu(bus);

//   // uint8_t whoAmI = 0;
//   // bool ok = mpu.whoAmI(whoAmI);
//   bool init =
//       mpu.init(Mpu6500GyroscopeRange::Dps250, Mpu6500AccelerometerRange::G2);

//   // (void)ok;
//   if (!init) {
//     while (true) {
//       /* breakpoint: init failed — check wiring/address */
//     };
//   }
//   mpu.calibrateGyroBias();
//   while (true) {
//     ImuSample data = mpu.read();
//     // breakpoint here: inspect data.accelG / data.gyroDps
//   }
// }

extern osMessageQueueId_t imuQueueHandle;

void runSensorTask() {
  I2CBus bus(hi2c1);
  Mpu6500 mpu(bus);
  ComplementaryFilter filter(0.98f, 0.001f);

  if (!mpu.init()) {
    for (;;) {
      osDelay(100);
      // init failed
    }
  }

  mpu.calibrateGyroBias();

  TickType_t lastWake = xTaskGetTickCount();
  const TickType_t period = pdMS_TO_TICKS(1);

  constexpr uint32_t TELEMETRY_DECIMATION = 20;
  uint32_t sampleCount = 0;
  uint32_t droppedCount = 0;

  for (;;) {
    ImuSample sample = mpu.read();
    filter.update(sample.accelG, sample.gyroDps);

    if (++sampleCount >= TELEMETRY_DECIMATION) {
      sampleCount = 0;
      ImuMsg msg;
      msg.accelG[0] = sample.accelG[0];
      msg.accelG[1] = sample.accelG[1];
      msg.accelG[2] = sample.accelG[2];
      msg.gyroDps[0] = sample.gyroDps[0];
      msg.gyroDps[1] = sample.gyroDps[1];
      msg.gyroDps[2] = sample.gyroDps[2];
      msg.roll = filter.roll();
      msg.pitch = filter.pitch();

      if (osMessageQueuePut(imuQueueHandle, &msg, 0, 0) != osOK) {
        droppedCount++;
      };
    }

    vTaskDelayUntil(&lastWake, period);
  }
}

void runTelemetryTask() {
  ImuMsg msg;
  for (;;) {
    if (osMessageQueueGet(imuQueueHandle, &msg, nullptr, osWaitForever) ==
        osOK) {
      ImuFrame frame = buildFrame(msg);
      HAL_UART_Transmit(&huart1, reinterpret_cast<uint8_t *>(&frame),
                        sizeof(ImuFrame), 50);
    }
  }
}