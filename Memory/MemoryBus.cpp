#include "../Cartridge/Cartridge.h"
#include "MemoryBus.h"
#include <cstring>

/*
Start	End	Description	                      Notes
0000	3FFF	16 KiB ROM bank 00	              From cartridge, usually a fixed bank
4000	7FFF	16 KiB ROM Bank 01–NN 	              From cartridge, switchable bank via mapper (if any)
8000	9FFF	8 KiB Video RAM (VRAM)	              In CGB mode, switchable bank 0/1
A000	BFFF	8 KiB External RAM	              From cartridge, switchable bank if any
C000	CFFF	4 KiB Work RAM (WRAM)	
D000	DFFF	4 KiB Work RAM (WRAM)	              In CGB mode, switchable bank 1–7
E000	FDFF	Echo RAM (mirror of C000–DDFF)	      Nintendo says use of this area is prohibited.
FE00	FE9F	Object attribute memory (OAM)	
FEA0	FEFF	Not Usable	                      Nintendo says use of this area is prohibited.
FF00	FF7F	I/O Registers	
FF80	FFFE	High RAM (HRAM)	
FFFF	FFFF	Interrupt Enable register (IE)	
*/

static Cartridge* cart = nullptr;

static uint8_t VRAM[0x2000]; //8000 - 9FFF
static uint8_t EXTERNAL_RAM[0x2000]; //A000 - BFFF
static uint8_t WRAM[0x2000]; //C000 - DFFF
static uint8_t OAM[0xA0]; //FE00-FE9F
static uint8_t IO[0x80]; //FF00-FF7F
static uint8_t HRAM[0x7F]; //FF80 - FFFE
static uint8_t IE_REGISTER = 0; //FFFF


void MemoryBus_init(Cartridge* c){
  cart = c;
  memset(VRAM, 0, sizeof(VRAM));
  memset(EXTERNAL_RAM, 0, sizeof(EXTERNAL_RAM));
  memset(WRAM, 0, sizeof(WRAM));
  memset(OAM, 0, sizeof(OAM));
  memset(IO, 0, sizeof(IO));
  memset(HRAM, 0, sizeof(HRAM));
  IE_REGISTER = 0;
}

uint8_t MemoryBus_read(uint16_t address){
  if(address <= 0x7FFF) return cart ? cart->read(address) : 0xFF; //0000-7FFF ROM
  if(address <= 0x9FFF) return VRAM[address - 0x8000];
  if(address <= 0xBFFF) return EXTERNAL_RAM[address - 0xA000];
  if(address <= 0xDFFF) return WRAM[address - 0xC000];
  if(address <= 0xFDFF) return WRAM[address - 0xE000]; //echo RAM
  if(address <= 0xFE9F) return OAM[address - 0xFE00];
  if(address <= 0xFEFF) return 0xFF;
  if(address <= 0xFF7F) return IO[address - 0xFF00];
  if(address <= 0xFFFE) return HRAM[address - 0xFF80];
  return IE_REGISTER;
}

void MemoryBus_write(uint16_t addr, uint8_t val){
  if(addr <= 0x7FFF){
    if(cart){
      cart->write(addr, val);//MBC bank-select write
    }
      return;
  }
  if(addr <= 0x9FFF){
    VRAM[addr - 0x8000] = val;
    return;
  }
  if(addr <= 0xBFFF){
    EXTERNAL_RAM[addr - 0xA000] = val;
    return;
  }
  if(addr <= 0xDFFF){
    WRAM[addr - 0xC000] = val;
    return;
  }
  if(addr <= 0xFDFF){
    WRAM[addr - 0xE000] = val;
    return;
  }
  if(addr <= 0xFE9F){
    OAM[addr - 0xFE00] = val;
    return;
  }
  if(addr <= 0xFEFF) return;
  if(addr <= 0xFF7F){
    IO[addr - 0xFF00] = val;
    return;
  }
  if(addr <= 0xFFFE){
    HRAM[addr - 0xFF80] = val;
    return;
  }
  IE_REGISTER = val;
}
