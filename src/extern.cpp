#include "extend.h"

extern "C" void waitMicrosecs(int microsecs)
{
  spinWait(microsecs);
}