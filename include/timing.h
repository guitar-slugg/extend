#ifndef TIMING_H
#define TIMING_H

#include <iostream>
#include <chrono>
#include <thread>

namespace extend
{

const int DEFUALT_SLEEP_THRESH_MS = 15;

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
  int availableSleep = microsecs / 1000 - sleepThresholdMilliseconds;
  if (availableSleep > 0)
  {
    std::this_thread::sleep_for(std::chrono::milliseconds(availableSleep));
  }

  while (getTimeMicroSecs() < goTime)
  {
    //spin
  };
};

void spinWait(int microsecs)
{
  spinWait(microsecs, DEFUALT_SLEEP_THRESH_MS);
}

class Stopwatch
{
public:
  Stopwatch();
  int PRINT_UNITS;
  void start();
  u_int64_t stop();
  u_int64_t stopAndPrintTime(std::string msg);
  u_int64_t timeFunction(void (*f)(), std::string msg);
  enum
  {
    TIME_UNIT_SECONDS,
    TIME_UNIT_MILLISECONDS,
    TIME_UNIT_MICROSECONDS
  };

private:
  u_int64_t microTime;
};

Stopwatch::Stopwatch()
{
  this->PRINT_UNITS = Stopwatch::TIME_UNIT_MILLISECONDS;
  this->microTime = 0;
}

void Stopwatch::start()
{
  this->microTime = getTimeMicroSecs();
};

u_int64_t Stopwatch::stop()
{
  return getTimeMicroSecs() - this->microTime;
};

u_int64_t Stopwatch::stopAndPrintTime(std::string msg)
{
  u_int64_t elapsedMicrosecinds = this->stop();
  double time=0; 
  std::string unitPrint = " microseconds"; 
  switch (this->PRINT_UNITS)
  {
  case Stopwatch::TIME_UNIT_SECONDS:
    unitPrint = " seconds";
    time= ((double)elapsedMicrosecinds) / 1000000;
    break;
  case Stopwatch::TIME_UNIT_MILLISECONDS:
    unitPrint = " milliseconds";
    time= ((double)elapsedMicrosecinds) / 1000;
    break;
  default:
    time=((double)elapsedMicrosecinds);
    break;
  }

  std::cout << msg << " : " <<time << unitPrint<< std::endl;
  return elapsedMicrosecinds;
};

u_int64_t Stopwatch::timeFunction(void (*f)(), std::string msg)
{
  this->start();
  f();
  return this->stopAndPrintTime(msg);
}

} // namespace extend

#endif