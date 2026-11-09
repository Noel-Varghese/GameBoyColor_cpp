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
static bool CGB_mode = false;

static uint8_t VRAM [2][0x2000]; //8000 - 9FFF
static uint8_t EXTERNAL_RAM[0x2000]; //A000 - BFFF
static uint8_t WRAM[8][0x2000]; //C000 - DFFF
/*
  Game Boy Color has 2 VRAM and 8 working RAMs primarly to overcome the 8-bit CPU's 16-bit addressing limiter
*/
static uint8_t OAM[0xA0]; //FE00-FE9F
static uint8_t IO[0x80]; //FF00-FF7F
static uint8_t HRAM[0x7F]; //FF80 - FFFE
static uint8_t IE_REGISTER = 0; //FFFF
/*
  Game Boy Color has two major visuals
  1.Backgrounds and window tiles
  2.sprites(characters, objects etc etc)
*/
static uint8_t BG_pallete[64];//for background and window tiles
/*
  GBC supports
  ->8 bg palletes
  ->4 color panels
  ->2 bytes per color
  hence, 8*4*2 = 64 elements
*/
static uint8_t OBJECT_pallete[64];//for sprites and objects

static int VRAM_BANK = 0;//stores which bank is currently selected
static int WRAM_BANK = 1;//stores which bank is currently selected

//h-blank direct memory access
static uint16_t hdma_source = 0;//stores where the data should be copied FROM
static uint16_t hdma_destination = 0;//stores where the data should be copied TO
static uint8_t hdma_remains = 0;//tracks how many blocks are left to transfer
static bool hdma_IsActive = false;//checks if transfer is active or not

//helps communicate serial output from the emulator
static std::string serial_log;
static std::string serial_line;

struct IODefault{
  uint8_t offset;
  uint8_t val;
};

static const IODefault PowerUP[] = {//default IO registers
  {0x00, 0xCF}, {0x01, 0x00}, {0x04, 0xAB}, {0x05, 0x00}, {0x06, 0x00}, {0x07, 0xF8}, {0x0F, 0xE1},
  {0x10, 0x80}, {0x11, 0xBF}, {0x12, 0xF3}, {0x13, 0xFF}, {0x14, 0xBF},
  {0x16, 0x3F}, {0x17, 0x00}, {0x18, 0xFF}, {0x19, 0xBF},
  {0x1A, 0x7F}, {0x1B, 0xFF}, {0x1C, 0x9F}, {0x1D, 0xFF}, {0x1E, 0xBF},
  {0x20, 0xFF}, {0x21, 0x00}, {0x22, 0x00}, {0x23, 0xBF},
  {0x24, 0x77}, {0x25, 0xF3}, {0x26, 0xF1},
  {0x40, 0x91}, {0x41, 0x85}, {0x42, 0x00}, {0x43, 0x00}, {0x44, 0x00}, {0x45, 0x00},
  {0x46, 0xFF}, {0x47, 0xFC}, {0x48, 0xFF}, {0x49, 0xFF}, {0x4A, 0x00}, {0x4B, 0x00},
};

static const IODefault PowerUpCGB[]={
  {0x02, 0x7F}, {0x4D, 0x00}, {0x68, 0xC8}, {0x6A, 0xD0}, {0x6C, 0x00}, {0x70, 0x00},
};

void MemoryBus_init(Cartridge* c, bool CGB){
  cart = c;
  CGB_mode = CGB;
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
