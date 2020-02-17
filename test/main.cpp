#include "extend.h"

void testSpinWait()
{
    extend::SpinWait::wait(1000);
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
    }
}

int main()
{
    extend::Stopwatch watch;  
    watch.timeFunction(testSpinWait, "testSpinWait");
    watch.timeFunction(testSHEM,"testSHEM");
    return 0; 
}