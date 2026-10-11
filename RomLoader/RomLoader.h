#ifndef ROMLOADER_H
#define ROMLOADER_H

#include <vector>
#include <string>
#include <cstdint>
#include "../Log/logger.h"
using namespace std;

class TRomLoader{
public:
  bool LoadRom(const string& filename, vector<uint8_t>& rom);
};

#endif
