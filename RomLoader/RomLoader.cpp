#include "RomLoader.h"
#include <fstream>//reads the file
using namespace std;

bool TRomLoader::LoadRom(const string& filename, vector<uint8_t>& rom){
  Log_INFO("Loading ROM: "+filename);
  ifstream file(filename, ios::binary | ios::ate);//ios::ate means at the end, start with the read pos at the end of the file
  if(!file){
    Log_ERROR("could not open the file at: "+ filename);
    return false;
  }
  streamsize size = file.tellg();//find the position of the read position, hence, due to ios::ate its the end of the file
  if(size < 0x150){
    Log_ERROR("file is too small to be a GBC file: "+ to_string(size)+ " bytes");
    return false;
  }
  rom.resize(static_cast<size_t>(size));//makes the file whole so there is enough space for the ROM
  file.seekg(0, ios::beg);
  if(!file.read(reinterpret_cast<char*>(rom.data()), size)){
      Log_ERROR("error while reading file");
      return false;
    }
    Log_INFO("ROM read successfully, size: " + to_string(size)+ " bytes");//the file closes by itself
    return true;
}
