#include "Cartridge.h"
#include <iostream>
#include "../Log/logger.h"

using namespace std;

TCartridge::TCartridge(vector<uint8_t> rom){
  m_rom = std::move(rom);//moves bytes from the parameter into m_rom, without copying
  
  if(m_rom.size() < 0x150){//checks the size of m_rom
    Log_ERROR("FILE IS TOO SMALL TO CONTAIN A HEADER: " + to_string(m_rom.size()) +" bytes" );//error logging
    return;
  }
  ParseHeader();
  m_valid = true;
  PrintSummary();
}

void TCartridge::ParseHeader(){
  ParseFlags();
  ParseTitle();
  ParseType();
  ParseRomSize();
  ParseRamSize();
  VerifyCheckSum();
}

void TCartridge::ParseFlags(){
  uint8_t cgbFlag = m_rom[0x143];//reads the byte at 0x143 into a local variable
  m_isCgb  = (cgbFlag == 0x80 || cgbFlag == 0xC0);//its a colot game if it is 0x80 or 0xC0
  m_isSgb = (m_rom[0x146] == 0x03);//byte at 0x146 is 0x03 for Super Game Boy support
}

void TCartridge::ParseTitle(){
  size_t end = m_isCgb ? 0x13F : 0x144;//color games reuses the last 4bytes of the title field
  for(size_t i=0x134; i< end; i++){
    uint8_t c = m_rom[i];
    if(c < 0x20 || c > 0x7E) break;
    m_title += static_cast<char>(c);//converts bytes to characters and adds it to the title
  }
}

void TCartridge::SetType(EMbcType type, bool ram, bool battery, bool timer, bool rumble){
  m_mbcType = type;
  m_hasRam = ram;
  m_hasBattery = battery;
  m_hasTimer = timer;
  m_hasRumble = rumble;
}

void TCartridge::ParseType(){
  uint8_t t = m_rom[0x147];
  switch(t){
  case 0x00: SetType(EMbcType::NONE, false, false, false, false);break;
  case 0x01: SetType(EMbcType::MBC1, false, false, false, false);break;
  case 0x02: SetType(EMbcType::MBC1, true, false, false, false);break;
  case 0x03: SetType(EMbcType::MBC1, true, true, false, false);break;
  case 0x05: SetType(EMbcType::MBC2, false, false, false, false);break;
  case 0x06: SetType(EMbcType::MBC2, false, true, false, false);break;
  case 0x0F: SetType(EMbcType::MBC3, false, true, true, false);break;
  case 0x10: SetType(EMbcType::MBC3, true, true, true, false);break;
  case 0x11: SetType(EMbcType::MBC3, false, false, false, false); break;
  case 0x12: SetType(EMbcType::MBC3, true,  false, false, false); break;
  case 0x13: SetType(EMbcType::MBC3, true,  true,  false, false); break;
  case 0x19: SetType(EMbcType::MBC5, false, false, false, false); break;
  case 0x1A: SetType(EMbcType::MBC5, true,  false, false, false); break;
  case 0x1B: SetType(EMbcType::MBC5, true,  true,  false, false); break;
  case 0x1C: SetType(EMbcType::MBC5, false, false, false, true);  break;
  case 0x1D: SetType(EMbcType::MBC5, true,  false, false, true);  break;
  case 0x1E: SetType(EMbcType::MBC5, true,  true,  false, true);  break;
  default:
    SetType(EMbcType::UNKNOWN, false, false, false, false);
    Log_WARN("UNSUPPORTED TYPE BYTE: " + to_string(static_cast<int>(t)));
    break;
  }
}

void TCartridge::ParseRomSize(){
  uint8_t code = m_rom[0x148];
  if(code <= 8){//reads the rom if cond is true banks = 2<<code.Shifts left by 1 doubles a number
    m_romBankCount = static_cast<size_t>(2) << code;
  }else{
    Log_WARN("UNKNOWN ROM SIZE CODE : "+to_string(static_cast<int>(code)));
  }
  if(m_romBankCount * 0x4000 != m_rom.size()){
    Log_WARN("header says "+ to_string(m_romBankCount * 0x4000)+ " bytes but the file is "+ to_string(m_rom.size()));
  }
}

void TCartridge::ParseRamSize(){
  uint8_t code = m_rom[0x149];
  switch(code){
  case 0x00: m_ramSize = 0;break;
  case 0x01: m_ramSize = 0;break;
  case 0x02: m_ramSize = 0x2000;break;
  case 0x03: m_ramSize = 0x8000;break;
  case 0x04: m_ramSize = 0x20000;break;
  case 0x05: m_ramSize = 0x10000;break;
  default:
    Log_WARN("unknown RAM size code: "+to_string(static_cast<int>(code)));
    break;
  }
  if(m_hasRam != (m_ramSize > 0)){
    Log_WARN("cartridge type and RAM size disagree");
  }
}

void TCartridge::VerifyCheckSum(){
  uint8_t x=0;
  for(size_t i = 0x134; i <= 0x14C; i++){
    x = static_cast<uint8_t>(x - m_rom[i] - 1);
  }
  m_checkSumValid = (x == m_rom[0x14D]);
  if(!m_checkSumValid){
    Log_WARN("header checksum does not match");
  }
}

const char* TCartridge::MbcName(EMbcType type){
  switch(type){
  case EMbcType::NONE: return "none (ROM only)";
  case EMbcType::MBC1: return "MBC1";
  case EMbcType::MBC2: return "MBC2";
  case EMbcType::MBC3: return "MBC3";
  case EMbcType::MBC5: return "MBC5";
  default:
    return "unknown";
  }
}

void TCartridge::PrintSummary() const{
  Log_INFO("Title: "+m_title);
  Log_INFO(string("MBC: ")+ MbcName(m_mbcType));
  Log_INFO("ROM banks: "+to_string(m_romBankCount));
  Log_INFO("RAM bytes: "+to_string(m_ramSize));
  Log_INFO(string("Battery: ") + (m_hasBattery ? "yes" : "no") + ", Timer: " + (m_hasTimer ? "yes" : "no") + ", Rumble: " + (m_hasRumble ? "yes" : "no"));
  Log_INFO(string("CGB: ") + (m_isCgb ? "yes" : "no") + ", SGB: " + (m_isSgb ? "yes" : "no"));
  Log_INFO(string("Header checksum: ") + (m_checkSumValid ? "OK" : "FAILED"));
}

bool TCartridge::IsValid() const{ return m_valid; }
string TCartridge::GetTitle() const{ return m_title; }
EMbcType TCartridge::GetMbcType() const{ return m_mbcType; }
bool TCartridge::HasRam() const{ return m_hasRam; }
bool TCartridge::HasBattery() const{ return m_hasBattery; }
bool TCartridge::HasTimer() const{ return m_hasTimer; }
bool TCartridge::HasRumble() const{ return m_hasRumble; }
bool TCartridge::IsCgb() const{ return m_isCgb; }
bool TCartridge::IsSgb() const{ return m_isSgb; }
size_t TCartridge::GetRomBankCount() const{ return m_romBankCount; }
size_t TCartridge::GetRamSize() const{ return m_ramSize; }
bool TCartridge::IsHeaderCheckSumValid() const{ return m_checkSumValid; }
 
uint8_t TCartridge::read(uint16_t address) const{
  if(address < m_rom.size()){
    return m_rom[address];
  }
  return 0xFF;
}
