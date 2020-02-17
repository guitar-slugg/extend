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

extern "C" int createSharedMem(const char * keyFile, int buffSize)
{
    return extend::createSharedMemory(keyFile, buffSize);
}

extern "C" void writeSharedMem(int shId, const char * message, int buffSize)
{
    extend::writeSharedMem(shId,message, buffSize);
}

