#pragma once
#include <stdint>

int coreInit();
void coreRun();
void coreShutdown();
void coreOnFrame();

void core_Advance_CPU_Clock(uint8_t clocks);
