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
    JSON jsonHandle; 
    for(int ii = 0 ; ii <20; ii ++ )
    {
        jsonHandle.put(std::to_string(ii)+ "int",ii);
        jsonHandle.putStr(std::to_string(ii)+ "str", "123");
    }

    std::string strr = jsonHandle.toJson();
    for(int ii=0; ii<1000; ii++)
    {
        jsonHandle.clear();
        jsonHandle.parse(strr);
    }
}

void testJson()
{
    JSON jsonHandle; 
    jsonHandle.put("intVal",3);
    jsonHandle.put("floatVal", 1.123);
    jsonHandle.putStr("strVal", "test123");
    jsonHandle.putBool("boolVal", true);

    std::vector<int> vals; 
    vals.push_back(1);vals.push_back(2);vals.push_back(3);
    jsonHandle.putArr("intArray", vals);

    std::vector<std::string> vals2; 
    vals2.push_back("one");vals2.push_back("two");vals2.push_back("three");
    jsonHandle.putStrArr("strArr", vals2);

    std::string jsonOut = jsonHandle.toJson();
    writeToFile("testJson.json", jsonOut);
    std::string jsonStr = readToString("testJson.json");

    for(int ii = 0 ; ii <20; ii ++ )
    {
        jsonHandle.put(std::to_string(ii)+ "int",ii);
        jsonHandle.putStr(std::to_string(ii)+ "int",std::to_string(ii));
    }

    jsonHandle.clear();
    jsonHandle.parse(jsonStr);
    std::string strVal = jsonHandle.get("strVal");
    std::string floatVal = jsonHandle.get("floatVal");
    std::vector<std::string> intArray = jsonHandle.getArr("intArray");
};

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    print("testing json");
    testJson();

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");
    watch.timeFunction(parseJson1000x, "parseJson1000x");

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