#include "heart.h"
#include "../Cartridge/Cartridge.h"
#include "../CPU/CPU.h"
#include "../Log/logger.h"
#inlcude "../CMDLineParser/CMDLineParser.h"
#include <SDL3/SDL.h>
#include <chrono>//provides tools for handling time, including durations (time intervals), time points (specific moments), and clocks (sources of time), which are used for measuring execution time, creating delays, and scheduling events
#include <thread>//provide multithreading
#include <string>

static constexpr int SCREEN_WIDTH = 160, SCREEN_HIEGHT = 144, SCALE = 4;
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
