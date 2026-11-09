#pragma once
#include <cstdint>

class Cartridge;
/*
  MemoryBus is the Game Boy's single 16-bit address space (0x0000 - 0xFFFF)
  Every Buyte the CPU and later the PPU, Timer, joypad etc reads or writes goes through here.
  The bus decides who owns that byte based on the address alone.
*/

void MemoryBus_init(Cartridge* cart);
uint8_t MemoryBus_read(uint16_t address);
void MemoryBus_write(uint16_t address, uint8_t value);
