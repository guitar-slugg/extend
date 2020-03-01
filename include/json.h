#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <vector>
#include <unordered_map>
#include <sstream>  //for std::istringstream
#include <algorithm>

// a do-it-yourself type json container
// simple to use,  and pretty fast, but lacks many fancy features

//todo-> not handling getting an item out of an array of objects

namespace extend
{
static const char SPACE = ' ';
static const char QUOTE = '"';
static const char COMMA = ',';
static const char COLIN = ':';
static const char OPEN_BRACKET = '[';
static const char CLOSE_BRACKET = ']';
static const char OPEN_CURLY = '{';
static const char CLOSE_CURLY = '}';

std::vector<std::string> & splitTrim(const std::string &s, char delim, std::vector<std::string> &elems) 
{
    std::stringstream ss(s+' ');
    std::string item;
    while(std::getline(ss, item, delim)) 
    {
        item.erase(std::remove(item.begin(), item.end(), '\n'), item.end());
        const int strBegin  = item.find_first_not_of(' '); 
        const int strEnd  = item.find_last_not_of(' '); 
        elems.push_back(item.substr(strBegin, strEnd- strBegin));
    }
    return elems;
}

class JsonContainer
{
public:
    JsonContainer(const char *jsonStrr)
    {
        this->parse(std::move(std::string(jsonStrr)));
    }

    JsonContainer(const std::string &&jsonStrr)
    {
        this->parse(std::move(jsonStrr));
    }

    JsonContainer(int bufferSize)
    {
        this->buffSize = bufferSize;
    }

    void parse(const std::string &&jsonStrr)
    {
        this->buffSize = jsonStrr.size();
        int siz = jsonStrr.size() - 1;
        int ii, start, stop, keylen, vallen, startCounter, endCounter;
        char startChar, endChar;
        std::string key;

        ii=0;
        ii = jsonStrr.find_first_of(QUOTE);

        //start parsing
        while (ii < siz)
        {
            start = 0;
            stop = 0;
            keylen = 0;
            vallen = 0;

            //find start of key
            while (ii < siz)
            {
                if (jsonStrr[ii] == QUOTE)
                {
                   break;
                }
                ++ii;
            };
            ++ii; // inc past quote
            start = ii;

            //find end of key
            while (ii < siz)
            {
                if (jsonStrr[ii] == QUOTE)
                {
                    break;
                }
                ++ii;
            };

            stop = ii - 1;
            keylen = stop - start + 1;
            key = jsonStrr.substr(start, keylen);

            //skip past colin
            while (ii < siz)
            {
                if (jsonStrr[ii] == COLIN)
                {
                    break;
                }
                ++ii;
            };
            ++ii; //inc one more

            //skip past whitespace to see what kind of values
            while (ii < siz)
            {
                if (jsonStrr[ii] != SPACE)
                {
                    break;
                }
                ++ii;
            };

            if (ii > siz - 1)
            {
                break;
            }

            startCounter = 0;
            endCounter = 0;

            switch (jsonStrr[ii])
            {
            case OPEN_CURLY:
                startChar = OPEN_CURLY;
                endChar = CLOSE_CURLY;
                break;
            case OPEN_BRACKET:
                startChar = OPEN_BRACKET;
                endChar = CLOSE_BRACKET;
                break;
            case QUOTE:
                startChar = '\0';
                endChar = COMMA;
                ++endCounter;
                break;
            default:
                startChar = '\0';
                endChar = COMMA;
                ++endCounter;
                break;
            }

            start = ii;
            //get to end of value
            while (ii < siz)
            {
                if (jsonStrr[ii] == startChar)
                {
                    ++endCounter;
                }
                if (jsonStrr[ii] == endChar)
                {
                    --endCounter;
                };
                if (endCounter == 0)
                {
                    break;
                }
                ++ii;
            };
            stop = ii;

            //do this for arrays
            if (endChar == CLOSE_BRACKET && jsonStrr[ii] != CLOSE_BRACKET)
            {
                ++stop;
            }

            //rewind if we hit the end of the thing
            if (endChar != CLOSE_CURLY)
            {
                while (jsonStrr[stop] == CLOSE_CURLY)
                {
                    --stop;
                };
            };

            vallen = stop - start + 1;
            this->add(key, std::move(jsonStrr.substr(start, vallen)));
        };
    }

    std::string get(const std::string &key)
    {
        std::string val = this->charMap[key];
        //clear trailing whitespace
        int ii = val.length() - 1;
        while (val[ii] == ' ')
        {
            --ii;
            if (ii < 0)
            {
                break;
            }
        }
        return val.substr(0, ii);
    }

    std::vector<std::string> getArray(const std::string &key)
    {
        std::vector<std::string> strVect;
        std::string val = get(key);

        const int strBegin = val.find_first_not_of(OPEN_BRACKET);
        const int strEnd = val.find_last_not_of(CLOSE_BRACKET);

        //clip brackets
        splitTrim(val.substr(strBegin, strEnd- strBegin), ',', strVect);
        return strVect;
    }

    std::vector<const std::string> getKeys()
    {
        std::vector<const std::string> keys;
        keys.reserve(this->charMap.size());

        for (auto kv : this->charMap)
        {
            keys.push_back(kv.first);
        }
        return keys;
    }

    void add(const std::string &key, const std::string &&val)
    {
        this->keyList.push_back(key);
        this->charMap[key] = std::move(val);
    }

    std::string stringify()
    {
        std::string out;
        int siz = this->keyList.size();
        out.reserve(this->buffSize + siz * 10);
        out += OPEN_CURLY;
        int count = 0;
        for (auto entry : this->keyList)
        {
            out += QUOTE;
            out += entry;
            out += QUOTE;
            out += COLIN;
            out += this->charMap[entry];

            if (count < siz - 1)
            {
                out += COMMA;
            }

            out += '\n';
            ++count;
        }

        out += CLOSE_CURLY;
        return out;
    }

    void clear()
    {
        this->keyList.clear();
        this->charMap.clear();
    }

private:
    int buffSize = 5200; //default buffer
    std::vector<const std::string> keyList;
    std::unordered_map<std::string, std::string> charMap;
};
}; // namespace extend

#endif