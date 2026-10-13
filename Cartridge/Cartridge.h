#ifndef CARTRIDGE_H
#define CARTRIDGE_H
#include <string>
#include <cstddef>
#include <vector>
#include <cstdint>

enum class EMbcType{
  NONE,
  MBC1,
    MBC2,
    MBC3,
    MBC5,
    UNKNOWN
    };
  class TCartridge{
  public:
    explicit TCartridge(std::vector<uint8_t> rom);

    bool IsValid() const;
    std::string GetTitle() const;
    EMbcType GetMbcType() const;
    bool HasRam() const;
    bool HasBattery() const;
    bool HasTimer() const;
    bool HasRumble() const;
    bool IsCgb() const;
    bool IsSgb() const;
    size_t GetRomBankCount() const;
    size_t GetRamSize() const;
    bool IsHeaderCheckSumValid() const;
    uint8_t read(uint16_t address) const;
  private:
    void ParseHeader();
    void ParseFlags();
    void ParseTitle();
    void ParseType();
    void ParseRomSize();
    void ParseRamSize();
    void VerifyCheckSum();
    void SetType(EMbcType type, bool ram, bool battery, bool timer, bool rumble);
    void PrintSummary() const;
    static const char* MbcName(EMbcType type);
    
    std::vector<uint8_t> m_rom;
    std::string m_title;
    EMbcType m_mbcType = EMbcType::UNKNOWN;
    bool m_valid = false;
    bool m_hasRam = false;
    bool m_hasBattery = false;
    bool m_hasTimer = false;
    bool m_hasRumble = false;
    bool m_isCgb = false;
    bool m_isSgb = false;
    size_t m_romBankCount = 0;
    size_t m_ramSize = 0;
    bool m_checkSumValid = false;
  };

#endif
  
