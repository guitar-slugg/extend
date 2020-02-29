#ifndef FILE_HH
#define FILE_HH

#include <iostream>

namespace extend
{

class File
{
public:
    template <typename T>
    static void write(const std::string &fileName, const T &data)
    {
        std::ofstream filestrm(fileName);
        filestrm << data << std::endl;
        filestrm.close();
    };

    static std::string read(const std::string fileName)
    {
        std::ifstream t(fileName);
        t.seekg(0, std::ios::end);
        size_t size = t.tellg();
        std::string buffer(size, ' ');
        t.seekg(0);
        t.read(&buffer[0], size);
        return buffer;
    };
};

} // namespace extend

#endif