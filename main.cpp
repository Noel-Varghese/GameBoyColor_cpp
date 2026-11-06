#include "HEART/heart.h"

int main(int argc, char** argv){
  int r = coreInit(argc, argv);
  if(r == CORE_FAIL)return 1;
  if(r == CORE_EXIT)return 0;

  coreRun();
  coreShutdown();
  return 0;
}
