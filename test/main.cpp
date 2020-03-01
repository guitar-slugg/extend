#include "extend.h"
#include <thread>   
#include <vector>   
#include <algorithm>

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
    File::write("logfileCOPY.log", Logger::getInstance()->getLog());
}

void servRun(SimpleRestServer srv)
{
    srv.run();
}

void testJson()
{
    std::string catalogStr = File::read("./test/catalog.json");
    std::string canadaStr = File::read("./test/canada.json");

    Stopwatch watch;

    watch.start();
    JsonContainer json(std::move(catalogStr));
    JsonContainer canadaJson(std::move(canadaStr));
    watch.stopAndPrintTime("catalog & canada  parse");

    print("GET SOME VALUES...");

    //check that we can extract a nested value
    JsonContainer nested(json.get("audienceSubCategoryNames"));
    print(nested.get("337100890"));

    JsonContainer events(json.get("events"));
   // print(events.stringify());

    JsonContainer someId(events.get("341181258"));
    print(someId.get("name"));
    print(someId.get("description"));
    for(auto val : someId.getArray("subTopicIds"))
    {
        print(val);
    }

    watch.start();
    json.stringify();
    watch.stopAndPrintTime("catalog stringify");

    json.add("dummyKey", "12345");
    File::write("testBig.json", json.stringify());

    std::string smallStrr = File::read("./test/small.json");
    watch.start();
    JsonContainer jsonSmall(std::move(smallStrr));
    watch.stopAndPrintTime("small parse");

    watch.start();
    jsonSmall.stringify();
    watch.stopAndPrintTime("small stringify");
}

int main()
{
    Logger * logger = Logger::getInstance();
    logger->info("starting tests");

    //test http server
    print("\ntesting SimpleRestServer \n");
    SimpleRestServer server(8080); 
    std::thread servThread(servRun, server);
    servThread.detach();
    system("curl -k localhost:8080/test/123");print("\n");
    server.stop();
    system("curl -k localhost:8080/flush");print("\n");

    Stopwatch watch; 
    watch.timeFunction(testSpinWait, "testSpinWait"); 
    watch.timeFunction(log1000Lines, "log1000Lines");
    watch.timeFunction(copyLogFile, "copyLogFile");
    testJson();
    
    logger->info("done");
    return 0; 
}