#include "../Cartridge/Cartridge.h"
#include "../Log/logger.h"
#include "MemoryBus.h"
#include <cstring>
#include <string>

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
//basically used for the Blargg test
static std::string serial_log;
static std::string serial_line;

struct IODefault{
  uint8_t offset;
  uint8_t val;
};

static const IODefault PowerUp[] = {//default IO registers
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
  VRAM_BANK = 0;
  WRAM_BANK = 1;
  hdma_source = 0;
  hdma_destination = 0;
  hdma_remains = 0;
  hdma_IsActive = false;
  serial_log.clear();
  serial_line.clear();
  //initializer
  for(const IODefault& d : PowerUp) IO[d.offset] = d.val;
  IO[0x02] = 0x7E;
  if(CGB_mode){
    for(const IODefault& d : PowerUpCGB) IO[d.offset] = d.val;
  }
}

bool MemoryBusCGB_Mode(){ return CGB_mode; }

void MemoryBusRequestInterrupt(uint8_t bit){
  IO[0x0f] |= (uint8_t)(1u << (bit & 0x07));
}

static uint8_t IO_read(uint16_t addr){
  switch(addr){
  case 0xFF00: return (uint8_t)(0xC0 | (IO[0x00] & 0x30) | 0x0F);
  case 0xFF02: return (uint8_t)(IO[0x02] | (CGB_mode ? 0x7C : 0x7E));
  case 0xFF0F: return (uint8_t)(0xE0 | IO[0x0F]);
  case 0xFF41: return (uint8_t)(0x80 | IO[0x41]);
  }
  if(!CGB_mode){
    if(addr == 0xFF4C || addr == 0xFF4D || addr == 0xFF4F || (addr >= 0xFF51 && addr <= 0xFF55) || (addr>= 0xFF68 && addr <= 0xFF6C) || addr == 0xFF70) return 0xFF;
    return IO[addr - 0xFF00];
  }
  switch(addr){
  case 0xFF4C: return 0xFF;
  case 0xFF4D: return (uint8_t)(0x7E | (IO[0x4D] & 0x81));
  case 0xFF4F: return (uint8_t)(0xFE | VRAM_BANK);
  case 0xFF51: case 0xFF52: case 0xFF53: case 0xFF54: return 0xFF;
  case 0xFF55: return hdma_IsActive ? (uint8_t)((hdma_remains - 1) & 0x7F) : 0xFF;
  case 0xFF68: return (uint8_t)(0x40 | IO[0x68]);
  case 0xFF69: return BG_pallete[IO[0x68] & 0x3F];
  case 0xFF6A: return (uint8_t)(0x40 | IO[0x6A]);
  case 0xFF6B: return OBJECT_pallete[IO[0x6A] & 0x3F];
  case 0xFF6C: return (uint8_t)(0xFE | (IO[0x6C] & 0x01));
  case 0xFF70: return (uint8_t)(0xF8 | (IO[0x70] & 0x07));
  }
  return IO[addr - 0xFF00];
}

/*OAM::Object attribute memory
  A Game Boy has 160 bytes of OAM
 */
static void OAM_dma(uint8_t high){//DMA::Direct memory access....this stores sprite information
  uint16_t source = (uint16_t)(high << 8);
  for(int i=0; i<0xA0;i++) OAM[i] = MemoryBus_read((uint16_t)(source + i));
}
//another DMA mechanism
static void hdma_CopyBlock(){//copies data into VRAM in 16_byte blocks
  for(int i=0; i< 16; i++){
    VRAM[VRAM_BANK][(hdma_destination + i) & 0x1FFF] = MemoryBus_read((uint16_t)(hdma_source + i));
  }
  hdma_source = (uint16_t)(hdma_source + 16);
  hdma_destination = (uint16_t)((hdma_destination + 16) & 0x1FF0);
}

/*
  HDMA :: H-Blank Direct Memory Access
  The game boy color LCD draws the screen one horizontal scanline @ a time
  after drawing a scanline, the PPU enters a short period called H-Blank(Horizontal Blank)
*/

void MemoryBus_hdma_hblank(){//transfers data into VRAM
  if(!hdma_IsActive)return;
  hdma_CopyBlock();
  hdma_remains--;
  if(hdma_remains == 0) hdma_IsActive = false;
}

static void serial_transfer(){//captures serial output
  char c = (char)IO[0x01];
  serial_log += c;
  if(c == '\n'){
    Log_INFO("serial: " + serial_line);
    serial_line.clear();//clears the current line
  }else{
    serial_line += c;
  }
  IO[0x01] = 0xFF;//resets the SB register
  IO[0x02] = (uint8_t)(IO[0x02] & ~0x80);
  MemoryBusRequestInterrupt(3);
}

static void IO_write(uint16_t addr, uint8_t val){
  switch(addr){
    //FF00 is the joypad register
  case 0xFF00://special DMG registers
    IO[0x00] = (uint8_t)(val & 0x30);
    return;
  case 0xFF02://serial control register
    IO[0x02] = val;
    if((val & 0x81) == 0x81) serial_transfer();
    return;
  case 0xFF04://divider register
    IO[0x04] = 0;
    return;
  case 0xFF0F://interrupt flag register
    IO[0x0F] = (uint8_t)(val & 0x1F);
    return;
  case 0xFF41://LCD STAT
    IO[0x41] = (uint8_t)((IO[0x41] & 0x07) | (val & 0x78));
    return;
  case 0xFF44:
    return;
  case 0xFF46://starts the OAM_dma
    IO[0x46] = val;
    OAM_dma(val);
    return;
  case 0xFF50:
    IO[0x50] = val;
    return;
  }
  if(!CGB_mode){
    if(addr == 0xFF4C || addr == 0xFF4D || addr == 0xFF4F || (addr >= 0xFF51 && addr <= 0xFF55) || (addr >= 0xFF68 && addr <= 0xFF6C) || addr == 0xFF70) return;
    IO[addr - 0xFF00] = val;
    return;
  }
  switch(addr){
  case 0xFF4C: return;
  case 0xFF4D: IO[0x4D] = (uint8_t)((IO[0x4D] & 0x80) | (val & 0x01)); return;
  case 0xFF4F:
    VRAM_BANK = val & 0x01;
    return;
  case 0xFF51:
    hdma_source = (uint16_t)((hdma_source & 0x00FF) | (val << 8));
    return;
  case 0xFF52:
    hdma_source = (uint16_t)((hdma_source & 0xFF00) | (val & 0xF0));
    return;
  case 0xFF53:
    hdma_destination = (uint16_t)((hdma_destination & 0x00FF) | ((val & 0x1F) << 8));
    return;
  case 0xFF54:
    hdma_destination = (uint16_t)((hdma_destination & 0xFF00) | (val & 0xF));
    return;
  case 0xFF55:
    if(hdma_IsActive && !(val & 0x80)){
      hdma_IsActive = false;
      return;
    }
    hdma_remains = (uint8_t)((val & 0x7F) + 1);
    if(val & 0x80){
      hdma_IsActive = true;
    }
    else{
      while(hdma_remains > 0){
	hdma_CopyBlock();
	hdma_remains--;
      }
    }
    return;
  case 0xFF68: IO[0x68] = (uint8_t)(val & 0xBF); return;
  case 0xFF69: {
    uint8_t idx = IO[0x68] & 0x3F;
    BG_pallete[idx] = val;
    if(IO[0x68] & 0x80) IO[0x68] = (uint8_t)(0x80 | ((idx + 1) & 0x3F));
    return;
  }
  case 0xFF6A: IO[0x6A] = (uint8_t)(val & 0xBF); return;
  case 0xFF6B: {
    uint8_t idx = IO[0x6A] & 0x3F;
    OBJECT_pallete[idx] = val;
    if(IO[0x6A] & 0x80) IO[0x6A] = (uint8_t)(0x80 | ((idx + 1) & 0x3F));
    return;
  }
  case 0xFF6C:
    IO[0x6C] = (uint8_t)(val & 0x01);
    return;
  case 0xFF70:
    IO[0x70] = (uint8_t)(val & 0x07);
    WRAM_BANK = (val & 0x07) ? (val & 0x07) : 1;
    return;
  }
  IO[addr - 0xFF00] = val;
}

uint8_t MemoryBus_read(uint16_t address){
  if(address < 0x8000) return cart ? cart->read(address) : 0xFF;
  switch(address >> 12){
  case 0x8: case 0x9: return VRAM[VRAM_BANK][address & 0x1FFF];
  case 0xA: case 0xB: return cart? cart->read_ram(address) : 0xFF;
  case 0xC: return WRAM[0][address & 0x0FFF];
  case 0xD: return WRAM[WRAM_BANK][address & 0x0FFF];
  case 0xE: return WRAM[0][address & 0x0FFF];
  }
  if(address < 0xFE00) return WRAM[WRAM_BANK][address & 0x0FFF];
  if(address < 0xFEA0) return OAM[address - 0xFE00];
  if(address < 0xFF00) return 0xFF;
  if(address < 0xFF80) return IO_read(address);
  if(address < 0xFFFF) return HRAM[address - 0xFF80];
  return IE_REGISTER;
}

const uint8_t* MemoryBus_VRAM(int bank){ return VRAM[bank & 1]; }
const uint8_t* MemoryBus_OAM(){ return OAM; }
const uint8_t* MemoryBus_BG_pallete(){ return BG_pallete; }
const uint8_t* MemoryBus_OBJECT_pallete(){ return OBJECT_pallete; }
const std::string& MemoryBus_SerialLog(){ return serial_log; }

void MemoryBus_write(uint16_t addr, uint8_t val){
  if(addr < 0x8000){
    if(cart) cart->write(addr, val);
    return;
  }
  switch(addr >> 12){
  case 0x8: case 0x9: VRAM[VRAM_BANK][addr & 0x1FFF] = val; return;
  case 0xA: case 0xB: if(cart) cart->write_ram(addr, val); return;
  case 0xC: WRAM[0][addr & 0x0FFF] = val; return;
  case 0xD: WRAM[WRAM_BANK][addr & 0x0FFF] = val; return;
  case 0xE: WRAM[0][addr & 0x0FFF] = val; return;
  }
  if(addr < 0xFE00){
    WRAM[WRAM_BANK][addr & 0x0FFF] = val;
    return;
  }
  if(addr < 0xFEA0){
    OAM[addr - 0xFE00] = val;
    return;
  }
  if(addr < 0xFF00) return;
  if(addr < 0xFF80){
    IO_write(addr, val);
    return;
  }
  if(addr < 0xFFFF){
    HRAM[addr - 0xFF80] = val;
    return;
  }
  IE_REGISTER = val;
}
