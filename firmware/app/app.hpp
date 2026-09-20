// firmware/app/app.hpp
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void appMain(void);
void runSensorTask();
void runTelemetryTask();

#ifdef __cplusplus
}
#endif