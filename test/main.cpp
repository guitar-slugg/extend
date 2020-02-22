#include "extend.h"
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

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");

    logger->info("done");

    SimpleHttpServer server(8080); 
    server.run();
    return 0; 
}