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
    File::write("logfileCOPY.log", Logger::getInstance()->getLog());
}

void servRun(SimpleRestServer srv)
{
    srv.run();
}

void serializeMedSizeJson()
{
    JsonObject json; 
    json.add("int1", 1234);
    json.add("int2", 45);
    json.add("double1", 1.3333);
    json.add("char1", "12345");
    json.add("str1", std::string("test123"));
    json.add("bool1", true);
    json.addNull("nullTest");

    std::vector<const char *> charArr;
    charArr.push_back("test123");
    charArr.push_back("hello");
    charArr.push_back("world");
    charArr.push_back("this is a test");
    charArr.push_back("of the emergency broadcast system");
    json.addCharArray("charArr",charArr);

    json.toJson();
}

const char * serializeJson()
{
    JsonObject json; 
    json.add("int1", 1234);
    json.add("int2", 45);
    json.add("double1", 1.3333);
    json.add("char1", "12345");
    json.add("str1", std::string("test123"));
    json.add("bool1", true);
    json.addNull("nullTest");

    std::vector<int> intArr;
    intArr.push_back(1);
    intArr.push_back(2);
    intArr.push_back(3);
    json.addNumericArray<int>("intArr", intArr, false);

    std::vector<double> dubArr; 
    dubArr.push_back(1.123);
    dubArr.push_back(2.232);
    dubArr.push_back(3.1232);
    json.addNumericArray<double>("dubArr", dubArr, true);

    std::vector<const char *> charArr;
    charArr.push_back("test123");
    charArr.push_back("hello");
    charArr.push_back("world");
    json.addCharArray("charArr",charArr);

    //add an object
    JsonObject objj(100); 
    objj.add("nestedInt", 1);
    objj.add("nestedDub", 1.23333);
    json.add("nestedObj", objj);

    //add object array 
    std::vector<JsonObject> objArr;
    JsonObject objA(200); 
    objA.add("key123", 123);
    objArr.push_back(objA);
    objArr.push_back(objA);
    objArr.push_back(objA);
    json.addObjectArray("objArr", objArr);

    //add custom text
    json.addBuffer("\"bruteForceAdd\":true,");

    //serialize
    const char * jsonStrr = json.toJson();

    return jsonStrr;
}


void serializeHUGEJson()
{
    JsonObject json(30000); 
    for(int ii=0; ii<1000; ii++)
    {
        json.add("test", "this");
        json.add("testNum", ii);
    }
    print("HUGE JSON SIZE (BYTES)->");
    const char * srrr = json.toJson();
    std::string strr(srrr);
    print(strr.size());
}

void nestingTest()
{
    JsonObject json(500);
    json.add("test1", 123);
    json.add("test2", "tet2");

    JsonObject nested1(200); 
    nested1.add("nestedVal1", 123);

    JsonObject nested2(200); 
    nested2.add("nestedVal2", 567);

    nested1.add("nestedObj2",nested2);
    json.add("nestObj1", nested1);

    JsonObject obj1Nested(json.findVal("nestObj1"), 100);
    //print(obj1Nested.findVal("nestedVal2"));

    json.modify("nestedVal2", "THIS_NEW_VAL");
    //print(json.toJson());
}


void findValueTest()
{
    const char * jsonStrr = serializeJson();
    JsonObject json(jsonStrr, std::strlen(jsonStrr) +100);
    json.add("findMee", 123456);

    //find array
    print(json.findVal("dubArr"));

    //find a nested value
    JsonObject nested(json.findVal("nestedObj"));
    print(nested.toJson());
    print(nested.findVal("nestedDub"));

    Stopwatch watch; 
    watch.start();
    json.findVal("findMee");
    watch.stopAndPrintTime("findValueTest");
}

void serialize1000X()
{
    for(int ii=0; ii<1000; ii++)
    {
        serializeJson();
    }
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
    watch.timeFunction(serializeMedSizeJson, "serializeMedSizeJson");
    watch.timeFunction(serializeHUGEJson, "serializeHUGEJson");
    watch.timeFunction(serialize1000X, "serialize1000X");
    findValueTest();
    nestingTest();

    File::write("test.json", std::string(serializeJson()));

    logger->info("done");
    return 0; 
}