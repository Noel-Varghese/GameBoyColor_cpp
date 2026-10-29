#include "heart.h"
#include "../Cartridge/Cartridge.h"
#include "../CPU/CPU.h"
#include "../Log/logger.h"
#include "../CMDLineParser/CmdLineParser.h"
#include <SDL3/SDL.h>
#include <chrono>//provides tools for handling time, including durations (time intervals), time points (specific moments), and clocks (sources of time), which are used for measuring execution time, creating delays, and scheduling events
#include <thread>//provide multithreading
#include <string>

static constexpr int SCREEN_WIDTH = 160, SCREEN_HEIGHT = 144, SCALE = 4;
static constexpr uint32_t CYCLES_PER_FRAME = 70224;//The clock speed is 4,194,304Hz. which is about 4.19million ticks per second. These ticks are T-cycles
/*
--->the screen has 154 scanlines per frame. 144 are visible rest are the verticle blank
--->each scanline takes 456 T-cycles.The Graphics chip takes time drawing lines
-->hence, 154*456 = 70224
*/
static constexpr int64_t FRAME_TIME_NS = 16742706;
/*
The CPU does 4,194,304 cycles in one second, so one frame's worth of cycles takes:
70,224/4,194,304 = 0.01674....seconds
multiply by 1,000,000,000 converts seconds to nano seconds
hece giving us 16742706
 */
//---->hence gameBoy runs at 59.73 fps

static Cartridge cart;
static SDL_Window* window = nullptr;
static SDL_Renderer* renderer = nullptr;
static bool quit_req = false;
static uint32_t frame_cycle = 0;
static std::chrono::steady_clock::time_point frame_begin;
static void coreSleepUntil(std::chrono::steady_clock::time_point target);

int coreInit(int argc, char** argv){
  TCmdLineParser parser;
  //takes the values and arguments from the command line
  if(!parser.ParseCmdLine(argc, argv)) return CORE_FAIL;
  if(parser.isHelpRequested()) return CORE_EXIT;
  if(parser.isLogLevelSet()){
    TLogger::getInstance()->setLogLevel(static_cast<ELogLevel>(parser.getLogLevel()));
  }
  if(!parser.isRomFileSet()){
    Log_ERROR("NO ROM GIVEN, RUN: ./gbc -r <game>.gbc");
    return CORE_FAIL;
  }
  //cartridge
  if(!SDL_Init(SDL_INIT_VIDEO)){
    Log_ERROR(std::string("SDL_Init failed!! : ")+SDL_GetError());
    return CORE_FAIL;
  }
  //creates window for the GBC #SDL
  window = SDL_CreateWindow("GBC", SCREEN_WIDTH * SCALE, SCREEN_HEIGHT * SCALE, 0);
  if(!window){
    Log_ERROR(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    coreShutdown();
    return CORE_FAIL;
  }
  renderer = SDL_CreateRenderer(window, nullptr);
  if(!renderer){
    Log_ERROR(std::string("SDL_CreateRenderer failed: ") + SDL_GetError());
    return CORE_FAIL;
  }
  /*
    CPU
   */
  CPU_init(&cart);
  return CORE_OK;
}

void coreRun(){
  CPU_reset();
  frame_cycle = 0;
  frame_begin = std::chrono::steady_clock::now();
  while(!quit_req){
    if(!CPU_step()) break;//unknown_opcode == stop
    if(frame_cycle >= CYCLES_PER_FRAME){
      frame_cycle -= CYCLES_PER_FRAME;//keep the leftover cycles
      coreOnFrame();
    }
  }
}

void coreShutdown(){
  if(renderer) SDL_DestroyRenderer(renderer);
  if(window) SDL_DestroyWindow(window);
  renderer = nullptr;
  window = nullptr;
  SDL_Quit();
}

void coreOnFrame(){
  SDL_Event e;
  while(SDL_PollEvent(&e)){
    if(e.type == SDL_EVENT_QUIT || e.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED) quit_req = true;
  }

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);//for the PPU
  SDL_RenderClear(renderer);
  SDL_RenderPresent(renderer);

  auto target = frame_begin + std::chrono::nanoseconds(FRAME_TIME_NS);
  if(std::chrono::steady_clock::now() < target) coreSleepUntil(target);
  frame_begin = std::chrono::steady_clock::now();
  
}

void core_advance_cpu_clock(uint8_t clocks){
  frame_cycle += clocks;
}

static void coreSleepUntil(std::chrono::steady_clock::time_point target){
  auto buffer = std::chrono::milliseconds(2);
  if(target - std::chrono::steady_clock::now() > buffer)std::this_thread::sleep_until(target-buffer);
  while(std::chrono::steady_clock::now() < target){
    std::this_thread::yield();
  }
}
