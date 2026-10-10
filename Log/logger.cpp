#include <iostream>
#include <memory>
#include <string>
#include "logger.h"

using namespace std;

shared_ptr<TLogger> TLogger::m_LoggerInstance = nullptr;

TLogger::TLogger():m_LogLevel(ELogLevel::DEBUG){//constructor

}

void TLogger::log(string msg, ELogLevel logLevel){
    string buff = "";
    if(logLevel<=m_LogLevel){
        switch(logLevel){
            case ELogLevel::ERROR:
                buff+="\033[31m[Error] ";
                break;
            case ELogLevel::WARN:
                buff+="\033[33m[Warn] ";
                break;
            case ELogLevel::INFO:
                buff += "\033[32m[Info] ";
                break;
            case ELogLevel::DEBUG://usually ignored due to its messy nature
                buff += "\033[0m[Debug] ";
                break;
            default:break;
        }
   	buff+= "\033[0m\n";
        cout<<buff;
    }
}

void TLogger::logAt(const string& msg, ELogLevel loglevel, const char* file, int line){
  if(logLevel == ELogLevel::NONE || logLevel > m_LogLevel){
    return;
  }
  const char* slash = strrchr(file, '/');
  const char* back = strrchr(file, '\\');
  if(back && (!slash || back > slash)) slash = back;
  const char* name = slash ? slash+1 : file;
  log(string(name) + ":" + to_string(line) + " " + msg, logLevel);
}

void TLogger::setLogLevel(ELogLevel logLevel){
    m_LogLevel = logLevel;
}

shared_ptr<TLogger> TLogger::getInstance(){
    if(m_LoggerInstance == nullptr){
        m_LoggerInstance = shared_ptr<TLogger> (new(TLogger));
    
    }
    return m_LoggerInstance;
}
