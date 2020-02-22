#include "extend.h"
#include <thread>         
using namespace extend;

void testSpinWait()
{
    SpinWait::wait(1000);
};

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
    writeToFile("logfileCOPY.log", Logger::getInstance()->getLog());
}

void servRun(SimpleRestServer srv)
{
    srv.run();
}

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");

    //test http server
    print("\ntesting SimpleRestServer \n");
    SimpleRestServer server(8080); 
    std::thread servThread(servRun, server);
    servThread.detach();
    system("curl -k localhost:8080/test/123");print("\n");
    server.stop();
    system("curl -k localhost:8080/flush");print("\n");

    logger->info("done");

    return 0; 
}