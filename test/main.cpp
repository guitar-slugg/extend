#include "extend.h"

void testFunc()
{
    extend::spinWait(1000);
}

void testSpinWait()
{
    int nIters =200; 
    int sleepTimeMicrosecs =20000;
    extend::Stopwatch watch;  

    watch.timeFunction(testFunc, "timed testFunc");

    double allTime =0.0; 
    for(int ii=0; ii<nIters; ii++)
    {
        watch.start();
        extend::spinWait(sleepTimeMicrosecs);
        allTime += watch.stop(); 
    }

    double avg = (allTime/nIters) - sleepTimeMicrosecs; 
    std::cout<<"spinWait avg accuracy: "<< avg << " microseconds" <<std::endl;
}


void testSHEM()
{
    std::cout<<"Shared memory test " <<std::endl;
    std::string keyFile = "hello.txt";
    int bufferSIze = 1024; 
    int shmemId = extend::createSharedMemory(keyFile.c_str(), bufferSIze);
    char buffer[bufferSIze]; 
 
    for(int ii=0 ; ii<10; ii++)
    {
        std::string str = "test" + std::to_string(ii); 
        extend::writeSharedMem(shmemId, str.c_str(), bufferSIze); 
        extend::readSharedMem(shmemId, buffer, bufferSIze);
        std::cout<<buffer<<std::endl;
        extend::spinWait(1000000);
    }
}

int main()
{
    testSpinWait();
    //testSHEM();
    return 0; 
}