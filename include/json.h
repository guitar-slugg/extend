#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <vector>
#include <cstdlib>

//light and fast JSON serializer and parser
//uses C style char buffer for speed
//you can overflow the buffer so chose your buffer size intelligently

namespace extend
{
class JsonObject
{
public:
    JsonObject()
    {
        this->buffer = new char[DEFAULT_BUFFER_SIZE];
        this->buffer[0] = '{';
        this->index = 1;
    }

    JsonObject(const char * jsonStr)
    {
        this->buffer = new char[DEFAULT_BUFFER_SIZE];
        this->index =0;
        addBuffer(jsonStr);
        this->buffer[this->index-1] = ',';
    }

    JsonObject(int maxBufferSize)
    {
        this->buffer = new char[maxBufferSize];
        this->buffer[0] = '{';
        this->index = 1;
    }

    JsonObject(const char * jsonStr, int maxBufferSize)
    {
        this->buffer = new char[maxBufferSize];
        this->index =0;
        addBuffer(jsonStr);
        this->buffer[this->index-1] = ',';
    }

    const char *toJson()
    {
        //clip if thre is comma at the end
        if (this->buffer[this->index - 1] == ',')
        {
            this->buffer[this->index - 1] = '}';
            this->buffer[this->index] = '\0';
            return this->buffer;
        };

        return this->buffer;
    }

    void add(const char *key, int value)
    {
        putKey(key);
        sprintf(this->numBuffer, "%d", value);
        putNumBuff();
    }

    void add(const char *key, double value)
    {
        putKey(key);
        sprintf(this->numBuffer, "%f", value);
        putNumBuff();
    }

    void add(const char *key, const char *value)
    {
        putKey(key);
        putCharBuffWithQuotes(value);
    }

    void add(const char *key, const std::string value)
    {
        add(key, value.c_str());
    }

    void add(const char *key, bool value)
    {
        putKey(key);
        if (value)
        {
            putCharBuffNoQuotes("true");
        }
        else
        {
            putCharBuffNoQuotes("false");
        }
    }

    void add(const char *key, JsonObject &objj)
    {
        putKey(key);
        putCharBuffNoQuotes(objj.toJson());
    }

    void addNull(const char *key)
    {
        putKey(key);
        putCharBuffNoQuotes("null");
    }

    template <class T>
    void addNumericArray(const char *key, const std::vector<T> &vect, bool floatingPoint)
    {
        putKey(key);
        this->buffer[this->index] = '[';
        ++this->index;

        const char * numType = "%d"; 
        if(floatingPoint)
        {
            numType= "%f";
        }

        for(auto ent : vect )
        {
            sprintf(this->numBuffer, numType, ent);
            putNumBuff();
        }

        --this->index;
        addBuffer("],",2);
    }

    void addCharArray(const char *key, const std::vector<const char *> &vect)
    {
        putKey(key);
        this->buffer[this->index] = '[';
        ++this->index;
        const char *type;
        for(auto ent : vect)
        {
            putCharBuffWithQuotes(ent);
        }

        --this->index;
        addBuffer("],",2);
    }

    void addBoolArray(const char *key, const std::vector<bool> &vect)
    {
        putKey(key);
        this->buffer[this->index] = '['; ++this->index;
        const char *type;
        for(auto ent : vect)
        {
            if(ent)
            {
                putCharBuffNoQuotes("true");
            }else
            {
                putCharBuffNoQuotes("false");
            }
        }
        --this->index;
        addBuffer("],",2);
    }

    void addObjectArray(const char *key, std::vector<JsonObject> &vect)
    {
        putKey(key);
        this->buffer[this->index] = '['; ++this->index;
        for(auto ent : vect)
        {
            putCharBuffNoQuotes(ent.toJson());
        }
        --this->index;
        addBuffer("],",2);
    };

    void addBuffer(const char *strr)
    {
        int len = std::strlen(strr);
        std::memcpy(&this->buffer[this->index], &strr[0], len);
        this->index += len;
        this->buffer[this->index] = NULL_CHAR;
    };

    void clear()
    {
        std::memset(this->buffer, '\0', std::strlen(this->buffer));
        this->buffer[0] = '{';
        this->index = 1;
    }

    const char *findVal(const char *key)
    {
        int startVal = 1;
        int stopVal = 0;
        int tempLen;
        int endCounter =0;
        int buffLen = std::strlen(this->buffer);
        while (this->buffer[startVal] != NULL_CHAR)
        {
            if(startVal > buffLen -2)
            {
                break;
            }

            //skip over "
            while (this->buffer[startVal] == QUOTE && this->buffer[startVal] != NULL_CHAR)
            {
                ++startVal;
            }

            //get to the end of the key
            stopVal = startVal;
            while (this->buffer[stopVal] != QUOTE)
            {
                ++stopVal;
            }

            tempLen = stopVal - startVal;
            std::memcpy(&keyBuffer, &this->buffer[startVal], tempLen);
            keyBuffer[tempLen] = NULL_CHAR;
            startVal = stopVal + 1;

            //check if this key is good
            if (std::strcmp(keyBuffer, key) == 0)
            {
                //skip over :
                while (this->buffer[startVal] != COLIN && this->buffer[startVal] != NULL_CHAR)
                {
                    startVal++;
                }
                startVal++;
                stopVal = startVal;

                //check if array or obj
                char endChar;
                char startChar;
                switch(this->buffer[stopVal])
                {
                    case OPEN_CURLY:
                        startChar = OPEN_CURLY;
                        endChar= CLOSE_CURLY;
                        break;
                    case OPEN_BRACKET:
                        startChar = OPEN_BRACKET;
                        endChar = CLOSE_BRACKET;
                        break;
                    default:
                        endChar = COMMA; 
                        ++endCounter;     
                }

                //get to end of value
                while (this->buffer[stopVal] != NULL_CHAR)
                {
                    if(this->buffer[stopVal] == startChar)
                    {
                        ++endCounter;
                    };
                    if(this->buffer[stopVal] == endChar)
                    {
                        --endCounter;
                    };

                    if(endCounter ==0)
                    {
                        break;
                    }
                    ++stopVal;
                }

                //add bracket if these types
                if(endChar==CLOSE_BRACKET || endChar == CLOSE_CURLY)
                {
                    ++stopVal;
                };

                //rewind if we hit the end of the thing
                if(endChar != CLOSE_CURLY)
                {
                    while(this->buffer[stopVal-1]==CLOSE_CURLY)
                    {
                        --stopVal;
                    };
                };

                tempLen = stopVal - startVal;
                memcpy(&keyBuffer, &this->buffer[startVal], tempLen);
                keyBuffer[tempLen] = NULL_CHAR;
                this->lastValStart = startVal; 
                this->lastValStop = stopVal;
                return keyBuffer;
            }
            else
            {
                //skip over un needed value
                while (this->buffer[startVal] != QUOTE && this->buffer[startVal] != NULL_CHAR)
                {
                    startVal++;
                }
            }
        }

        //save this for later internally if we need it for modification 
        this->lastValStart = 0; 
        this->lastValStop = 0;
        return "NOT_FOUND";
    }

    bool modify(const char * key, const char * newValue)
    {
        if(std::strcmp(this->findVal(key), "NOT_FOUND") == 0)
        {
            return false;
        };

        int len = this->index - this->lastValStop; 
        int newValLen = strlen(newValue);

        char tempEndBuff[len];
        //copy buffer end to temp 
        std::memcpy(&tempEndBuff, &this->buffer[lastValStop], len);

        //write in new val
        int ii=0;
        while(newValue[ii] != '\0')
        {
            this->buffer[lastValStart + ii] = newValue[ii];
            ++ii;
        }

        //paste back in the rest of the buffer 
        std::memcpy(&this->buffer[lastValStart+ newValLen], &tempEndBuff, len);
        this->index = strlen(this->buffer);
        return true;
    }

private:
    char * buffer;
    static const int DEFAULT_BUFFER_SIZE = 5200;
    static const char QUOTE = '"';
    static const char COMMA = ',';
    static const char COLIN = ':';
    static const char OPEN_BRACKET = '[';
    static const char CLOSE_BRACKET = ']';
    static const char OPEN_CURLY = '{';
    static const char CLOSE_CURLY = '}';
    static const char NULL_CHAR = '\0';
    char numBuffer[200];
    char keyBuffer[200];
    int index=0;
    int lastValStart =0;
    int lastValStop = 0;

    //save the time of doing strlen
    void addBuffer(const char *strr, int len)
    {
        std::memcpy(&this->buffer[this->index], &strr[0], len);
        this->index += len; 
        this->buffer[this->index] = NULL_CHAR;
    }

    void putKey(const char *key)
    {
        this->buffer[this->index] = '"';++this->index;
        addBuffer(key);
        addBuffer("\":",2);
    };

    void putNumBuff()
    {
        addBuffer(this->numBuffer);
        this->buffer[this->index] = ',';++this->index;
    }

    void putCharBuffNoQuotes(const char *value)
    {
        addBuffer(value);
        this->buffer[this->index] = ',';++this->index;
    }

    void putCharBuffWithQuotes(const char *value)
    {
        this->buffer[index] = '"';++this->index;
        addBuffer(value);
        addBuffer("\",",2);
    }
};
} // namespace extend

#endif