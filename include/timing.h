#ifndef TIMING_H
#define TIMING_H

#include<iostream>
#include <chrono>
#include <thread>  

const int DEFUALT_SPIN_WAIT = 15; 

u_int64_t getTimeMs()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
};

//sleep buffer time, and spin remaining time
void spinWait(int ms, int sleepThreshold)
{
  u_int64_t baseTime = getTimeMs();
  u_int64_t goTime = baseTime + ms; 

  //sleep if we can 
  int availableSleep = ms - sleepThreshold; 
  if(availableSleep > 0 )
  {
      std::this_thread::sleep_for (std::chrono::milliseconds(availableSleep));
  }

  while(getTimeMs() < goTime)
  {
    //spin 
  };

};

void spinWait(int ms)
{
  spinWait(ms, DEFUALT_SPIN_WAIT);
}

#endif