#include "simlib.h"

extern "C" void spinWaitMs(int ms)
{
  spinWait(ms);
}