#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <map>

namespace extend
{

//very limited json support, but usefull for small things
class JSON
{
public:
    void parse(std::string jsonStr)
    {
        const char *charArr = cleanStr(jsonStr).c_str();
        int len = strlen(charArr);
        int counter = 1;
        bool readingKey = false;
        bool readingVal = false;
        bool recursiveType = false;
        int keyStart = 1;
        int keyEnd = 0;
        int keyLen = 0;
        int valStart = 0;
        int valEnd = 0;
        int valLen = 0;

        while (true)
        {
            //start of key
            if (!readingVal)
            {
                if (charArr[counter] == '"' && charArr[counter + 1] != ':')
                {
                    readingKey = true;
                    readingVal = false;
                    keyStart = counter + 1;
                }

                //end of key
                if (charArr[counter] == '"' && charArr[counter + 1] == ':')
                {
                    readingKey = false;
                    readingVal = true;
                    keyEnd = counter - 1;

                    //get key subbuffer
                    keyLen = keyEnd - keyStart + 1;
                    std::string keyStr = buffSubStr(charArr, keyStart, keyLen);
                    keysOrder.push_back(keyStr);
                }
            }

            //reading value
            if (readingVal)
            {
                if (charArr[counter] == ':' && charArr[counter - 1] == '"')
                {
                    valStart = counter + 1;
                    if (charArr[valStart] == '[' || charArr[valStart] == '{')
                    {
                        recursiveType = true;
                    }
                    else
                    {
                        recursiveType = false;
                    }
                }

                if (recursiveType)
                {
                    //end of var
                    if ((charArr[counter] == ']' && charArr[counter + 1] == ',') || (charArr[counter] == '}' && charArr[counter + 1] == ','))
                    {
                        valEnd = counter;
                        valLen = valEnd - valStart + 1;
                        std::string valStr = buffSubStr(charArr, valStart, valLen);
                        dataMap.emplace(keysOrder.back(), valStr);
                        readingVal = false;
                    }

                    if (counter == len - 1)
                    {
                        valEnd = counter - 1;
                        valLen = valEnd - valStart + 1;
                        std::string valStr = buffSubStr(charArr, valStart, valLen);
                        dataMap.emplace(keysOrder.back(), valStr);
                        readingVal = false;
                    }
                }
                else
                {
                    //end of var
                    if ((charArr[counter] == ',' && charArr[counter + 1] == '"') || (counter == len - 1))
                    {
                        valEnd = counter - 1;
                        valLen = valEnd - valStart + 1;
                        std::string valStr = buffSubStr(charArr, valStart, valLen);
                        dataMap.emplace(keysOrder.back(), valStr);
                        readingVal = false;
                    }
                }
            }

            counter++;
            if (counter >= len)
            {
                break;
            }
        }
    }

    std::string toJson()
    {
        std::stringstream ss;
        ss << "{" << std::endl;
        for(int ii =0; ii< this->keysOrder.size(); ii++)
        {
            ss << "\"" << this->keysOrder.at(ii) << "\":" << this->dataMap[this->keysOrder.at(ii)];
            if(ii<this->keysOrder.size() -1)
            {
                ss<<",";
            }
            ss<<std::endl;
        }
        ss << "}";
        return ss.str();
    }

    void putStr(const std::string &key, const std::string &val)
    {
        this->keysOrder.push_back(key);
        this->dataMap.emplace(key, "\"" + val + "\"");
    }

    void putBool(const std::string &key, bool val)
    {
        this->keysOrder.push_back(key);
        if (val)
        {
            this->dataMap.emplace(key, "true");
        }
        else
        {
            this->dataMap.emplace(key, "false");
        }
    }

    void putStrArr(const std::string &key, const std::vector<std::string> &arr) 
    {
        std::stringstream ss;        
        this->keysOrder.push_back(key);
        ss<<"["; 
        for(int ii=0; ii< arr.size(); ii++)
        {
            ss<<"\""<<arr.at(ii)<<"\"";
            if(ii<arr.size() -1)
            {
                ss<<",";
            }
        }
        ss<<"]"; 
        this->dataMap.emplace(key, ss.str());
    }

    template <typename T>
    void putArr(const std::string &key, const std::vector<T> &arr) 
    {
        std::stringstream ss;        
        this->keysOrder.push_back(key);
        ss<<"["; 
        for(int ii=0; ii< arr.size(); ii++)
        {
            ss<<arr.at(ii);
            if(ii<arr.size() -1)
            {
                ss<<",";
            }
        }
        ss<<"]"; 
        this->dataMap.emplace(key, ss.str());
    }

    template <typename T>
    void put(const std::string &key, T val)
    {
        std::stringstream ss;
        ss << val;
        this->keysOrder.push_back(key);
        this->dataMap.emplace(key, ss.str());
    };

    std::string get(const std::string &key)
    {
        return this->dataMap[key];
    };

    std::vector<std::string> getArr(const std::string &key)
    {
        std::vector<std::string> vect;
        int last=1; 
        int len =0;
        bool newStr = false;
        std::string vals = cleanStr(this->get(key));

        for(int ii=1; ii< vals.length(); ii++)
        {
            len++;
            if(vals.at(ii) == ',' || ii== vals.length()-1)
            {
                vect.push_back(vals.substr(last, len-1));
                last = ii+1;
                len =0;
            }
        }
        return vect;
    } 

    void clear()
    {
        this->dataMap.clear();
        this->keysOrder.clear();
    }

private:
    std::vector<const std::string> keysOrder;
    std::map<const std::string, const std::string> dataMap;

    const std::string cleanStr(std::string str)
    {
        str.erase(remove(str.begin(), str.end(), ' '), str.end());
        str.erase(remove(str.begin(), str.end(), '\r'), str.end());
        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
        return str;
    };

    const std::string buffSubStr(const char *buffer, int start, int len)
    {
        char subbuff[len];
        memcpy(subbuff, &buffer[start], len);
        subbuff[len] = '\0';
        return std::string(subbuff);
    }
};

}; // namespace extend

#endif
