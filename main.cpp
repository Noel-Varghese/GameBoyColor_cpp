#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "Log/logger.h"
#include "RomLoader/RomLoader.h"

using namespace std;
//argc = how many things were typed on the command line
//argv = the things as text
int main(int argc, char* argv[]){
  Log_INFO("starting the emulator");
  if(argc < 2){
    Log_ERROR("No ROM given");
    return 1;
  }
  string romPath = argv[1];
  vector<uint8_t> rom;
  TRomLoader loader;
  if(!loader.LoadRom(romPath, rom)){
    Log_ERROR("failed to load rom, shutting down");
    return 1;
  }
  string title;
  for(int i= 0x0134; i<0x0144; i++){
    if(rom[i] == 0) break;
    title += static_cast<char>(rom[i]);
  }
  Log_INFO("Game Title: " + title);
  return 0;
}
