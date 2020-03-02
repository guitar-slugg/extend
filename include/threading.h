#ifndef THREADING_H
#define THREADING_H

#include<vector>
#include <thread>
#include <queue>
#include <iostream>
#include <mutex>
#include <condition_variable>

#include "timing.h"

namespace extend
{

bool ready = false;

class Runnable 
{
    public: 
        virtual void run()=0;
};

class Worker
{
    public:
        std::mutex m;
        std::condition_variable cond_var;

    void work()
    {
        // Wait until main() sends data
        std::unique_lock<std::mutex> lk(m);
        cond_var.wait(lk, []{return ready;});
 
        // after the wait, we own the lock.
        std::cout << "Worker thread is processing data\n";

        // Manual unlocking is done before notifying, to avoid waking up
        // the waiting thread only to block again (see notify_one for details)
        lk.unlock();
        cond_var.notify_one();    
    }   
};


class TheadPool
{
    public: 

        std::queue<Runnable> todo;
        void run()
        {
            std::thread thred; 
            
        }

};

class Runner : public Runnable
{
    public:
        void run()
        {
            std::cout<<"bleh" <<std::endl;
        }
};

static void workerTest()
{
    Worker wk; 
    std::thread workerThr(&Worker::work, &wk);
    // send data to the worker thread
    {
        std::lock_guard<std::mutex> lk(wk.m);
        std::cout << "Waiting a second \n";
        SpinWait::wait(1000000);
        ready = true;
        std::cout << "main() signals data ready for processing\n";
    }
    wk.cond_var.notify_one();
    workerThr.join();
};



};







#endif