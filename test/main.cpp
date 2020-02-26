#include "extend.h"
#include <thread>   
#include <vector>      
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


void parseJson1000x()
{
    //build a normal size json
    JsonObject jsonObj;
    jsonObj.put("testInt",123);
    jsonObj.put("testInt2",567034);
    jsonObj.put("testStr","123");

    JsonArray newArr1; 
    newArr1.put(1);
    newArr1.put(2);
    newArr1.put(3);
    newArr1.put(4);
    jsonObj.put("arrayObj", newArr1);

    jsonObj.put("testAStringAgain","12345678");
    jsonObj.put("testAStringAgain2","12345678");
    jsonObj.put("testBool",true);
    jsonObj.put("testBool2",false);

    JsonObject nestedObj;
    nestedObj.put("something", 1);
    nestedObj.put("somethingElse", true);
    nestedObj.put("somethingElseAgain", "WiffleBat");

    JsonObject nestedObj2; 
    nestedObj2.put("nested1", 2);
    nestedObj2.put("nested2", true);
    nestedObj2.put("nested3", "data");
    nestedObj.put("secondaryNested",nestedObj2 );

    jsonObj.put("nestedObj",nestedObj );

    JsonArray newArr2; 
    newArr2.put("1");
    newArr2.put("2");
    newArr2.put("3");
    jsonObj.put("arrayObj2", newArr2);

    std::string strr = jsonObj.toJson();
    print(strr);
    print(JsonObject::parseStr(strr).toJson());

    Stopwatch watch; 
    watch.start();
    for(int ii=0; ii<1000; ii++)
    {
        JsonObject::parseStr(strr);
    }
    watch.stopAndPrintTime("time to parse 1000 jsons");
}

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");
    parseJson1000x();

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