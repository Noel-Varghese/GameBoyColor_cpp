#pragma once
#include <cstdint>

//coreInit() return values
enum CoreResult{
  CORE_FAIL = -1,//if something goes wrong it exits with an error
  CORE_OK = 0,//ready to run
  CORE_EXIT = 1//noting to run (eg --help), main exists normally
};

int coreInit(int argc, char** argv);//parse args, loads ROM, open window and sets the CPU
void coreRun();//main loop
void coreShutdown();//releases or shutsdown everything safely
void coreAdvanceCPUClock(uint8_t clocks);//CPU call for memory access
void coreOnFrame();//once per frame, it manages all the events, drawing, speed limiting
