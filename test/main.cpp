#include "extend.h"

using namespace extend;

void testSpinWait()
{
    SpinWait::wait(1000);
};

void testSHEM()
{
    PRINT("Shared memory test ");
    std::string keyFile = "hello.txt";
    int bufferSIze = 1024; 
    int shmemId = createSharedMemory(keyFile.c_str(), bufferSIze);
    char buffer[bufferSIze]; 
 
    for(int ii=0 ; ii<10; ii++)
    {
        std::string str = "test" + std::to_string(ii); 
        writeSharedMem(shmemId, str.c_str(), bufferSIze); 
        readSharedMem(shmemId, buffer, bufferSIze);
        std::cout<<buffer<<std::endl;
        PRINT(buffer);
    }
}

void log1000Lines()
{
    Logger * logger = Logger::getInstance();
    logger->PRINT_LEVEL = Logger::LOG_LEVEL::WARN;
    for(int ii=0 ; ii<1000; ii++)
    {
        logger->info("logged something");
    };
     logger->PRINT_LEVEL = Logger::LOG_LEVEL::INFO;
}

void copyLogFile()
{
    dumpToFile("logfileCOPY.log", Logger::getInstance()->getLog());
}

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");

    logger->info("done");

    //watch.timeFunction(testSHEM,"testSHEM");
    return 0; 
}