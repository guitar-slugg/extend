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
  extend::spinWait(microsecs);
}

extern "C" void readShMem()
{
    for(int ii=0 ; ii<10; ii++)
    {
        std::string str = "test" + std::to_string(ii); 
        extend::shareFunc(false, str.c_str());
        extend::spinWait(1000000);
    }
}