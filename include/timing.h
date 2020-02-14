#ifndef TIMING_H
#define TIMING_H

#include<iostream>
#include <chrono>
#include <thread>  

namespace extend
{

const int DEFUALT_SLEEP_THRESH_MS= 15; 

u_int64_t getTimeMilliSecs()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
};

u_int64_t getTimeMicroSecs()
{ 
  return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count(); 
}

//High accuracy wait. Sacrifices some cpu cycles for accuracy
void spinWait(int microsecs, int sleepThresholdMilliseconds)
{
  u_int64_t baseTime = getTimeMicroSecs();
  u_int64_t goTime = baseTime + microsecs;

  //sleep if we can 
  int availableSleep = microsecs/1000 - sleepThresholdMilliseconds; 
  if(availableSleep > 0 )
  {
      std::this_thread::sleep_for (std::chrono::milliseconds(availableSleep));
  }

  while(getTimeMicroSecs() < goTime)
  {
    //spin 
  };

};

void spinWait(int microsecs)
{
  spinWait(microsecs, DEFUALT_SLEEP_THRESH_MS);
}

}

#endif