#ifndef TIMING_H
#define TIMING_H

#include <iostream>
#include <chrono>
#include <thread>

namespace extend
{
class Stopwatch
{
public:
  static u_int64_t getTimeMilliSecs()
  {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
  };

  static u_int64_t getTimeMicroSecs()
  {
    return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
  }

  void start()
  {
    this->microTime = getTimeMicroSecs();
  };

  u_int64_t stop()
  {
    return getTimeMicroSecs() - this->microTime;
  };

  u_int64_t stopAndPrintTime(std::string msg)
  {
    u_int64_t elapsedMicrosecinds = this->stop();
    double timeMs = ((double)elapsedMicrosecinds) / 1000;
    std::cout << msg << " -> " << elapsedMicrosecinds << " microseconds (" << timeMs << " ms)" << std::endl;
    return elapsedMicrosecinds;
  };

  u_int64_t timeFunction(void (*f)(), std::string msg)
  {
    this->start();
    f();
    return this->stopAndPrintTime(msg);
  };

private:
  u_int64_t microTime = 0;
};

class SpinWait
{
public:
  static const int DEFUALT_SLEEP_THRESH_MS = 15;

  static void wait(int microsecs, int sleepThreshold)
  {
    u_int64_t baseTime = Stopwatch::getTimeMicroSecs();
    u_int64_t goTime = baseTime + microsecs;

    //sleep if we can
    int availableSleep = microsecs / 1000 - sleepThreshold;
    if (availableSleep > 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(availableSleep));
    }

    while (Stopwatch::getTimeMicroSecs() < goTime)
    {
      //spin
    };
  };

  static void wait(int microsecs)
  {
    u_int64_t baseTime = Stopwatch::getTimeMicroSecs();
    u_int64_t goTime = baseTime + microsecs;

    //sleep if we can
    int availableSleep = microsecs / 1000 - DEFUALT_SLEEP_THRESH_MS;
    if (availableSleep > 0)
    {
      std::this_thread::sleep_for(std::chrono::milliseconds(availableSleep));
    }

    while (Stopwatch::getTimeMicroSecs() < goTime)
    {
      //spin
    };
  };
};

} // namespace extend

#endif