#pragma once
#include <cstdint>
#include <string>

class Cartridge;
/*
  MemoryBus is the Game Boy's single 16-bit address space (0x0000 - 0xFFFF)
  Every Buyte the CPU and later the PPU, Timer, joypad etc reads or writes goes through here.
  The bus decides who owns that byte based on the address alone.
*/

void MemoryBus_init(Cartridge* cart, bool CGB);
uint8_t MemoryBus_read(uint16_t address);
void MemoryBus_write(uint16_t address, uint8_t value);

bool MemoryBusCGB_Mode();
void MemoryBusRequestInterrupt(uint8_t bit);
void MemoryBus_hdma_hblank();

const uint8_t* MemoryBus_VRAM(int blank);
const uint8_t* MemoryBus_OAM();
const uint8_t* MemoryBus_BG_pallete();
const uint8_t* MemoryBus_OBJECT_pallete();

const std::string& MemoryBus_SerialLog();
