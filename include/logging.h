#ifndef LOGGING_H
#define LOGGING_H

#include <iostream>
#include <fstream>

namespace extend
{

template<typename T>
void PRINT(T s)
{
    std::cout << s << std::endl;
}

template<typename T>
void dumpToFile(const std::string & fileName, const T & s)
{
    std::ofstream filestrm(fileName);
    filestrm << s << std::endl;
    filestrm.close();
}

std::string readToString(const std::string & fileName)
{
    std::ifstream t(fileName);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size); 
    return buffer;
}

class Logger
{
public:

    static Logger* getInstance()
    {
        if(instance ==0)
        {
            instance = new Logger();
        }
       return instance;
    };

    enum LOG_LEVEL
    {
        TRACE,
        DEBUG,
        INFO,
        WARN,
        ERROR
    };

    LOG_LEVEL FILE_LEVEL = LOG_LEVEL::INFO; 
    LOG_LEVEL PRINT_LEVEL = LOG_LEVEL::INFO; 

    void trace(const std::string &msg)
    {
        this->log(
            "trace | " +  msg, 
            LOG_LEVEL::TRACE  >= this->PRINT_LEVEL,
            LOG_LEVEL::TRACE >= this->FILE_LEVEL
            );
    };

    void debug(const std::string &msg)
    {
        this->log(
            "debug | " + msg, 
            LOG_LEVEL::DEBUG  >= this->PRINT_LEVEL,
            LOG_LEVEL::DEBUG >= this->FILE_LEVEL
            );
    };

    void info(const std::string &msg)
    {
        this->log(
            "info | " + msg, 
            LOG_LEVEL::INFO  >= this->PRINT_LEVEL,
            LOG_LEVEL::INFO >= this->FILE_LEVEL
            );
    };

    void warn(const std::string &msg)
    {
        this->log(
            "warn | " + msg, 
            LOG_LEVEL::WARN  >= this->PRINT_LEVEL,
            LOG_LEVEL::WARN >= this->FILE_LEVEL
            );
    };

    void error(const std::string &msg)
    {
         this->log(
             "error | "+ msg, 
             LOG_LEVEL::ERROR  >= this->PRINT_LEVEL,
             LOG_LEVEL::ERROR >= this->FILE_LEVEL
             );
    };

    void setFile(const std::string &fileName)
    {
        this->fileName = fileName;
        this->outFile.close();
        this->outFile = std::ofstream(fileName);
    };

    ~Logger()
    {
        this->outFile.close();
    };

    std::string getLog()
    {
        return readToString(this->fileName);
    }

private:
    Logger()
    {
        this->setFile(this->fileName);
    }

    void log(const std::string &msg, bool print, bool file)
    {
        if(print){std::cout << msg << std::endl;};
        if(file){this->outFile<<msg<<std::endl;};
    }

    std::ofstream outFile;
    std::string fileName= "logfile.log";
    static Logger * instance;
};

Logger* Logger::instance = 0;

} // namespace extend

#endif