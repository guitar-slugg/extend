#include "extend.h"

extern "C" void bareWrapper()
{
  //to test overhead 
}

extern "C" void hello()
{
  std::cout<<"hello"<<std::endl;
}

extern "C" void waitMicrosecs(int microsecs)
{
  extend::SpinWait::wait(microsecs);
}