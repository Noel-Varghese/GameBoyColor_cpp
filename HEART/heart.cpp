#include "heart.h"
#include "cart.h"
#include "cpu.h"
#include <chrono>//provides tools for handling time, including durations (time intervals), time points (specific moments), and clocks (sources of time), which are used for measuring execution time, creating delays, and scheduling events
#include <thread>//provide multithreading

int coreInit(){
  cart_printInfo();
}

void corRun(){
  cpu_reset();
}

void coreShutdown(){
  
}

void core_On_Frame(){
  
}
