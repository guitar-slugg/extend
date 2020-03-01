#ifndef TIMING_H
#define TIMING_H

#include <iostream>
#include <chrono>
#include <thread>
#include <iomanip>
#include <ctime>
#include <sstream>

namespace extend
{
class Stopwatch
{
public:
  static std::string timeStamp()
  {
    auto t = std::time(nullptr);
    auto tm = *std::localtime(&t);

    std::ostringstream oss;
    oss << std::put_time(&tm, "%d-%m-%Y %H-%M-%S");
    return oss.str();
  }

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

  u_int64_t stopAndPrintTime(const std::string & msg)
  {
    u_int64_t elapsedMicrosecinds = this->stop();
    double timeMs = ((double)elapsedMicrosecinds) / 1000;
    std::cout << msg << " -> " << elapsedMicrosecinds << " microseconds (" << timeMs << " ms)" << std::endl;
    return elapsedMicrosecinds;
  };

  u_int64_t timeFunction(void (*f)(), const std::string & msg)
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