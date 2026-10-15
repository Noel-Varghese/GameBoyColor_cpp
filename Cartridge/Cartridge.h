#ifndef CARTRIDGE_H
#define CARTRIDGE_H

#include <cstdint>

const int MAX_CART_SIZE = 8*1024*1024; //defining the max cartridge size can be either 1MB or 8MB

//defining the Cartridge header
struct Cartridge_info{
  uint8_t EntryPoint[4];//hardware reserves 4bytes
  uint8_t Nintendo_logo[48];//The cartridge header contains a 48-byte Nintendo logo field, and the boot ROM checks this data
  uint8_t title[15];//constains title of the game in upper ascii
  bool is_CGB;
  uint8_t New_Licensee_Code[2];//because the licencee code is a two bit ascii code 
  bool is_SGB;
  uint8_t Cartridge_type;
  uint8_t ROM_size;
  uint8_t RAM_size;
  uint8_t Destination_code;
  uint8_t Old_licensee_code;
  uint8_t Mask_ROM_verNummer;
  uint8_t Header_Checksum;
  uint16_t Global_Checksum;
}

#endif
