#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <sstream>
#include <vector>
#include <stdlib.h>

//todo : handle arrays of objects 

namespace extend
{    
class JsonPrim
{
public:
    JsonPrim(const char *val)
    {
        if (val[0] != '\"')
        {
            this->val = "\"" + std::string(val) + "\"";
        }
        else
        {
            this->val = std::string(val);
        }
    }

    JsonPrim(const std::string &val)
    {
        if (val.at(0) != '\"')
        {
            this->val = "\"" + val + "\"";
        }
        else
        {
            this->val = val;
        }
    }

    JsonPrim(int val)
    {
        std::stringstream ss;
        ss << val;
        this->val = ss.str();
    }

    JsonPrim(double val)
    {
        std::stringstream ss;
        ss << val;
        this->val = ss.str();
    }

    JsonPrim(bool val)
    {
        if (val)
        {
            this->val = "true";
        }
        else
        {
            this->val = "false";
        }
    }

    JsonPrim()
    {
        this->val = "null";
    }

    static JsonPrim parseStr(const std::string &val)
    {
        if (val.length() < 1)
        {
            return JsonPrim();
        }

        if (val.at(0) == '\"')
        {
            return JsonPrim(val);
        };

        if (val.compare("true") == 0)
        {
            return JsonPrim(true);
        }

        if (val.compare("false") == 0)
        {
            return JsonPrim(false);
        }

        if (val.find('.') != std::string::npos)
        {
            return JsonPrim(atof(val.c_str()));
        }

        return JsonPrim(atoi(val.c_str()));
    }

    std::string toJson()
    {
        return this->val;
    }

    std::string toStr()
    {
        return this->val.substr(1, this->val.size() - 2);
    }

    int toInt()
    {
        return atoi(this->val.c_str());
    }

    double toDouble()
    {
        return atof(this->val.c_str());
    }

    bool toBool()
    {
        if (this->val.compare("true") == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

private:
    std::string val;
};

class JsonArray
{
public:
    void put(JsonPrim val)
    {
        this->prims.push_back(val);
    }

    std::string toJson()
    {
        std::stringstream ss;
        ss << "[";
        for (int ii = 0; ii < this->prims.size(); ii++)
        {
            ss << this->prims.at(ii).toJson();

            if (ii < this->prims.size() - 1)
            {
                ss << ",";
            }
        }
        ss << "]";
        return ss.str();
    }

    JsonPrim get(int index)
    {
        return this->prims.at(index);
    }

private:
    std::vector<JsonPrim> prims;
};

class JsonObject
{
public:
    void put(const std::string &key, JsonPrim val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, int val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, double val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, const std::string &val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, const char *val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, bool val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::PRIMATIVE, this->prims.size()));
        this->prims.push_back(JsonPrim(val));
    }

    void put(const std::string &key, JsonObject val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::OBJECT, this->objects.size()));
        this->objects.push_back(val);
    }

    void put(const std::string &key, JsonArray val)
    {
        this->jsonEntries.push_back(JsonEntry(key, JsonEntry::ARRAY, this->arrays.size()));
        this->arrays.push_back(val);
    }

    JsonPrim getPrimative(const std::string &key)
    {
        for (auto entry : this->jsonEntries)
        {
            if (entry.name.compare(key) == 0)
            {
                return this->prims.at(entry.listIndex);
            }
        }

        return JsonPrim();
    }

    JsonObject getObject(const std::string &key)
    {
        for (auto entry : this->jsonEntries)
        {
            if (entry.name.compare(key) == 0)
            {
                return this->objects.at(entry.listIndex);
            }
        }

        return JsonObject();
    }

    JsonArray getArray(const std::string &key)
    {
        for (auto entry : this->jsonEntries)
        {
            if (entry.name.compare(key) == 0)
            {
                return this->arrays.at(entry.listIndex);
            }
        }

        return JsonArray();
    }

    const std::string toJson()
    {
        std::stringstream ss;
        ss << "{" << std::endl;
        int counter = 0;
        for (auto entry : this->jsonEntries)
        {
            switch (entry.type)
            {
            case JsonEntry::PRIMATIVE:
                ss << "\"" << entry.name << "\":" << this->prims.at(entry.listIndex).toJson();
                break;
            case JsonEntry::OBJECT:
                ss << "\"" << entry.name << "\":" << this->objects.at(entry.listIndex).toJson();
                break;
            case JsonEntry::ARRAY:
                ss << "\"" << entry.name << "\":" << this->arrays.at(entry.listIndex).toJson();
                break;
            }

            if (counter < this->jsonEntries.size() - 1)
            {
                ss << ",";
            }
            ss << std::endl;
            counter++;
        }
        ss << "}";
        return ss.str();
    }

    static JsonObject parseStr(const std::string & jsonStr)
    {
        JsonObject root;
        std::string fullStr = cleanStr(jsonStr);
        int index = 1;
        int strLen = fullStr.length();
        int valLen;
        int keyLen;
        std::string valueStr;
        std::string keyStr;

        while (index <= strLen-1)
        {
            keyLen = 0;

            //skip over any ""
            while(fullStr.at(index + keyLen) != '"')
            {
                index++;

                if(index+keyLen > strLen -2)
                {
                    break;
                }
            }

            //find end of key 
            while (fullStr.at(index + keyLen) != ':')
            {
                if(index+keyLen > strLen -2)
                {
                    break;
                }
                keyLen++;
            }

            if(index+keyLen > strLen -2)
            {
                break;
            }

            //trim off quotes of key 
            keyStr = fullStr.substr(index+1, keyLen - 2);

            //advance index past key
            index += keyLen + 1;

            valLen = 0;
            switch (fullStr.at(index))
            {
            case '{':
                {
                while (fullStr.at(index + valLen) != '}')
                {
                    if(index+valLen > strLen -2)
                    {
                        break;
                    }
                    valLen++;
                };

                valLen++; //account for bracket
                valueStr = fullStr.substr(index , valLen);
                JsonObject objj = JsonObject::parseStr(valueStr);
                root.put(keyStr, objj);

                }
                break;

            case '[':
                {
                //read array
                while (fullStr.at(index + valLen) != ']')
                {
                    if(index+valLen > strLen -2)
                    {
                        break;
                    }
                    valLen++;
                };

                //clip off bracket
                valueStr = fullStr.substr(index + 1, valLen - 1);
                JsonArray jsonArr;

                std::stringstream ss(valueStr);
                while (ss.good())
                {
                    std::string substr;
                    getline(ss, substr, ',');
                    jsonArr.put(JsonPrim::parseStr(substr));
                }

                valLen++; //need to account for bracket

                root.put(keyStr, jsonArr);
                }
                break;
            default:
                {
                //read primative
                while (fullStr.at(index + valLen) != ',')
                {
                    if(index+valLen > strLen -2)
                    {
                        break;
                    }
                    valLen++;
                };

                valueStr = fullStr.substr(index, valLen);
                root.put(keyStr, JsonPrim::parseStr(valueStr));
                }
                break;
            };
            index += valLen + 1;
        }

        return root;
    }

    void clear()
    {
        this->jsonEntries.clear();
        this->prims.clear();
        this->objects.clear();
        this->arrays.clear();
    }

private:
    class JsonEntry
    {
    public:
        enum TYPE
        {
            PRIMATIVE,
            ARRAY,
            OBJECT
        };

        TYPE type;
        std::string name;
        int listIndex;

        JsonEntry(const char *key, TYPE type, int index)
        {
            this->name = std::string(key);
            this->type = type;
            this->listIndex = index;
        }

        JsonEntry(const std::string &key, TYPE type, int index)
        {
            this->name = key;
            this->type = type;
            this->listIndex = index;
        }
    };

    static const std::string cleanStr(std::string str)
    {
        str.erase(remove(str.begin(), str.end(), '\r'), str.end());
        str.erase(remove(str.begin(), str.end(), '\n'), str.end());
        return str;
    };

    std::vector<JsonEntry> jsonEntries;
    std::vector<JsonPrim> prims;
    std::vector<JsonObject> objects;
    std::vector<JsonArray> arrays;
};

}; // namespace extend

#endif
