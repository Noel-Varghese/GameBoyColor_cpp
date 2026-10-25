#include "CmdLineParser.h"
#include <stdexcept>

using namespace std;

TCmdLineParser::TCmdLineParser():m_RomFileName(""), m_logLevel(-1), m_HelpReq(false){
}
void TCmdLineParser::PrintHelpMessage(){
  cout<<"Usage: gbc [options] [rom]" << endl;
  cout<<"options: "<<endl;
  cout<<"\t -h, --help \t\t Display this help information"<<endl;
  cout<<"\t -r, --RomFileName \t\t set the rom file path to be used"<<endl;
  cout<<"\t -l, --loglevel \t\t Set the desired log level [NONE = 0, ERROR = 1, WARN = 2, INFO = 3, DEBUG = 4] "<<endl;
  cout<<"A path with no option in front of it is considered as the rom"<<endl;
}

void TCmdLineParser::SetRomFileName(const string& name){
  m_RomFileName = name;
}

void TCmdLineParser::SetLogLevel(int LogLevel){
  m_logLevel = LogLevel;
}

bool TCmdLineParser::ParseCmdLine(int argc, char** argv){
  for(int i = 1; i<argc;i++){
    string arg = argv[i];
    if(arg == "-h" || arg == "--help"){
      m_HelpReq = true;
      PrintHelpMessage();
      return true;
    }else if(arg == "-r" || arg == "--rom" || arg == "--RomFileName"){
      if(i+1 >= argc){
	Log_ERROR(arg + " needs a value between e.g " + arg + " game.gbc or game.gb or game.dmg");
	return false;
      }
      SetRomFileName(argv[++i]);
    }
    else if(arg == "-l" || arg == "--loglevel"){
      if(i+1 >= argc){
	Log_ERROR(arg + " needs a value between 0 and 4");
	return false;
      }
      string value = argv[++i];
      size_t pos = 0;
      int level = -1;
      try{
	level = stoi(value, &pos);
      }
      catch(const exception&){
	Log_ERROR("Log level must be a number between 0 and 4: "+ value);
	return false;
      }
      if(pos != value.size() || level < 0 || level>4){
	Log_ERROR("log level must be between 0 and 4: "+value);
	return false;
      }
      SetLogLevel(level);
    }
    else if(!arg.empty() && arg[0] != '-'){
      SetRomFileName(arg);
    }else{
      Log_ERROR("unknown parameter: "+arg);
      return false;
    }
  }
  return true;
}

bool TCmdLineParser::isRomFileSet() const{
  return !m_RomFileName.empty();
}
string TCmdLineParser::getRomFileName() const{
  return m_RomFileName;
}
bool TCmdLineParser::isLogLevelSet() const{
  return m_logLevel != -1;
}
int TCmdLineParser::getLogLevel() const{
  return m_logLevel;
}
bool TCmdLineParser::isHelpRequested() const{
  return m_HelpReq;
}
