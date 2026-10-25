#include "../Log/logger.h"
#include "Cartridge.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <cstdio>


bool Cartridge::load(const char* path){
  loaded = false;
  rom.clear();
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  if(!file){
    Log_ERROR("COULD NOT OPEN THE FILE");
    return false;
  }
  std::streamsize size = file.tellg();
  if(size < 0x150 || size > MAX_CART_SIZE){
    Log_ERROR("INVALID ROM SIZE");
    return false;
  }
  rom.resize(size);
  file.seekg(0, std::ios::beg);
  if(!file.read(reinterpret_cast<char*>(rom.data()), size)){
    Log_ERROR("FAILED TO LOAD ROM");
    rom.clear();
    return false;
  }

  info = cart_type_data[Cartridge_Type()];//points to the Cartridge type
  loaded = true;
  Log_INFO("ROM loaded: "+std::string(path)+" ( "+std::to_string((long long)size)+" bytes ");
  return true;
  
}

void Cartridge::print_info(){
  if(!loaded){
    Log_WARN("NO ROM LOADED");
    return;
  }

  std::vector<uint8_t> t = Title();
  std::string title(t.begin(), t.end());
  title.resize(strlen(title.c_str()));

  char buff[64];
  Log_INFO("Title: "+title);
  snprintf(buff, sizeof(buff), "Type: %02X (%s)", Cartridge_Type(), info.readable_name);
  Log_INFO(std::string(buff));
  
  snprintf(buff, sizeof(buff), "CGB flag: %02X", CGB_Flag());
  Log_INFO(std::string(buff));

  snprintf(buff, sizeof(buff), "SGB Flag: %02X", SGB_Flag());
  Log_INFO(std::string(buff));

snprintf(buff, sizeof(buff), "ROM Size: %02X", ROM_Size());
  Log_INFO(std::string(buff));

snprintf(buff, sizeof(buff), "RAM Size: %02X", RAM_Size());
  Log_INFO(std::string(buff));

  snprintf(buff, sizeof(buff), "Destination: %02X", Destination_code());
  Log_INFO(std::string(buff));

  snprintf(buff, sizeof(buff), "Version: %02X", Mask_Rom_Version());
  Log_INFO(std::string(buff));

  snprintf(buff, sizeof(buff), "Checksum: %02X", Checksum());
  Log_INFO(std::string(buff));

  snprintf(buff, sizeof(buff), "Global_Checksum: %02X", Global_Checksum());
  Log_INFO(std::string(buff));
  
  uint8_t sum=0;
  for(int i = 0x0134;i <= 0x014C;i++) sum = sum - rom[i] - 1;
  if(sum != Checksum()) Log_WARN("HEADER CHECKSUM DOES NOT MATCH, THIS ROM MAY BE CORRUPT");
}

uint8_t Cartridge::read(uint16_t address) const{
  if(address >= rom.size()) return 0xFF;
  return rom[address];
}
