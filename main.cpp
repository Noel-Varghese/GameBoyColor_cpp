#include <iostream>
#include <SDL3/SDL.h>
#include "Log/logger.h"
#include "Cartridge/Cartridge.h"
#include "CMDLineParser/CmdLineParser.h"
#include "CPU/CPU.h"

int main(int argc, char**argv){
  TCmdLineParser parser;
  if(!parser.ParseCmdLine(argc, argv)) return 1;
  if(parser.isHelpRequested()) return 0;

  if(parser.isLogLevelSet()){
    TLogger::getInstance()->setLogLevel(static_cast<ELogLevel>(parser.getLogLevel()));
  }
  if(!parser.isRomFileSet()){
    Log_ERROR("NO ROM GIVEN, RUN:gbc game.gbc");
    return 1;
  }

  CPU_reset();
  CPU_read();
  CPU_write();
  
  Cartridge cart;
  if(!cart.load(parser.getRomFileName().c_str())) return 1;
  cart.print_info();
  return 0;
}
