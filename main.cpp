#include <iostream>
#include <string>
#include <vector>
#include <cstdint>
#include "Log/logger.h"
#include "RomLoader/RomLoader.h"
#include "CMDLineParser/CmdLineParser.h"
#include "Cartridge/Cartridge.h"

using namespace std;
//argc = how many things were typed on the command line
//argv = the things as text
int main(int argc, char* argv[]){
  Log_INFO("starting the emulator");
  TCmdLineParser parser;
  if(!parser.ParseCmdLine(argc, argv)) return 1;
  if(parser.isHelpRequested()) return 0;
  if(parser.isLogLevelSet()) TLogger::getInstance()->setLogLevel(static_cast<ELogLevel>(parser.getLogLevel()));
  if(!parser.isRomFileSet()){
    Log_ERROR("No ROM given, use -r <file> or -h for help");
    return 1;
  }
  string romPath = parser.getRomFileName();
  vector<uint8_t> rom;
  TRomLoader loader;
  if(!loader.LoadRom(romPath, rom)){
    Log_ERROR("failed to load rom, shutting down");
    return 1;
  }
  TCartridge cart(move(rom));
  if(!cart.IsValid()){
    Log_ERROR("INVALID ROM, SHUTTING DOWN");
    return 1;
  }
  return 0;
}
