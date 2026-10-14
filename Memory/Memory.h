#ifndef MEMORY_H
#define MEMORY_H

#include <array>
#include <cstdint>
#include <cstdint>

class Cartridge;
class Processor;
class Video;
class IORegisters;

class Memory{
public:
  static constexpr size_t kVRAMBankSize = 0x2000;
  static constexpr size_t kVRAMBankCount = 2;
  static constexpr size_t KWRAMBankSize = 0x1000;
  static constexpr size_t kWRAMBankCount = 8;
  static constexpr size_t kOamSize = 0xA0;
  static constexpr size_t kHRAMSize = 0x7F;

  Memory();
  ~Memory() = default
    Memory(const Memory&) = delete;
  Memory& operator=(const Memory&) = delete;

  void SetCartridge(Cartridge* cartridge);
  void SetProcessor(Processor* processor);
  void SetVideo(Video* video);
  void SetIO(IORegisters* io);
  void reset(bool isCGB, bool isSGB = false);

  uint8_t read(uint16_t adress) const;
  void write(uint16_t address, uint8_t value);

  const uint8_t* GetVRAM() const;
  const uint8_t* GetVRAMBank0() const;
  const uint8_t* GetVRAMBank1() const;
  const uint8_t* GetOAM() const;
  const uint8_t* GetWRAM0() const;
  const uint8_t* GetWRAM1() const;
  const uint8_t* GetHRAM() const;

  bool isCGB() const;
  int GetVRAMBank() const;
  int GetWRAMBank() const;

private:
  uint8_t Read_IO(uint16_t address) const;
  void WriteId(uint16_t address, uint8_t value);
  void StartOAMDMA(uint8_t sourceHigh);

  
  
};

#endif
