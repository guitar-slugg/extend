#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <fstream>

namespace extend
{

static std::mutex printLockMutex; 

template <typename T>
static void print(T s)
{
    std::lock_guard<std::mutex> guard(printLockMutex);
    std::cout << s << std::endl;
};

template <typename T>
void writeToFile(const std::string &fileName, const T &s)
{
    std::ofstream filestrm(fileName);
    filestrm << s << std::endl;
    filestrm.close();
}

std::string readToString(const std::string &fileName)
{
    std::ifstream t(fileName);
    t.seekg(0, std::ios::end);
    size_t size = t.tellg();
    std::string buffer(size, ' ');
    t.seekg(0);
    t.read(&buffer[0], size);
    return buffer;
}

}

#endif