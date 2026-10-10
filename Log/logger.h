#ifndef LOGGER_H
//IF NOT DEFINED IS A PREPROCESSOR DIRECTIVE USED FOR CONDITIONAL COMPILATION
//IT CHECKS WHETHER A SPECIFIED MACRO HAS NOT BEEN DEFINED EARLIER IN THE FILE OR IN AN INCLUDED FILE
#define LOGGER_H
#include <iostream>
#include <string>
#include <fstream>
#include <memory>
using namespace std;

enum class ELogLevel{
    NONE = 0,
    ERROR = 1,
    WARN = 2,
    INFO = 3,
    DEBUG = 4
};

class TLogger{
  ELogLevel m_LogLevel;
  static shared_ptr<TLogger> m_LoggerInstance;//manages the logger system
 public:
  TLogger();
  void log(const string& msg, ELogLevel loglevel);
  void logAt(const string& msg, ELogLevel loglevel, const char* file, int line);
  void setLogLevel(ELogLevel loglevel);
  static shared_ptr<TLogger> getInstance();
};

#define Log_ERROR(msg) TLogger::getInstance()->logAt((msg), ELogLevel::ERROR, __FILE__, __LINE__)
#define Log_WARN(msg) TLogger::getInstance()->logAt((msg), ELogLevel::WARN, __FILE__, __LINE__)
#define Log_INFO(msg) TLogger::getInstance()->logAt((msg), ELogLevel::INFO, __FILE__, __LINE__)

#ifdef LOG_DISABLE_DEBUG
  #define LOG_DEBUG(msg) ((void)0)
#else
  #define LOG_DEBUG(msg) TLogger::getInstance()->logAt((msg), ELogLevel::DEBUG, __FILE__, __LINE__)
#endif

#endif

