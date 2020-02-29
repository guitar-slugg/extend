#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <vector>
#include <unordered_map>

// a do-it-yourself type json container
// simple to use,  and pretty fast, but lacks many fancy features 

//todo-> does not really handle arrays well yet.. may want to add a getArray(index) type method

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

class JsonContainer
{
    public:
        JsonContainer(const char * jsonStrr)
        {
            this->parse(std::move(std::string(jsonStrr)));
        }

        JsonContainer(const std::string && jsonStrr)
        {
            this->parse(std::move(jsonStrr));
        }

        void parse(const std::string &&jsonStrr)
        {
            this->buffSize = jsonStrr.size();
            int siz = jsonStrr.size()-1;
            int ii, start, stop, keylen, vallen, startCounter, endCounter;
            char startChar, endChar; 
            std::string key;

            ii=0;
            //skip to first key
            while(ii < siz)
            {
                if(jsonStrr[ii]== QUOTE)
                {
                    break;
                }
                ++ii;
            };

            //start parsing
            while(ii < siz)
            {
                start = 0;
                stop =0;
                keylen =0; 
                vallen =0;

                //find start of key
                while(ii < siz)
                {
                    if(jsonStrr[ii]== QUOTE)
                    {
                        break;
                    }
                    ++ii;
                };

                ++ii;// inc past quote
                start =ii;

                //find end of key
                while(ii < siz)
                {
                    if(jsonStrr[ii] == QUOTE)
                    {
                        break;
                    }
                    ++ii;
                };

                stop = ii-1; 
                keylen = stop-start+1;
                key = jsonStrr.substr(start, keylen);

                //skip past colin
                while(ii < siz)
                {
                    if(jsonStrr[ii] == COLIN)
                    {
                        break;
                    }
                    ++ii;
                };
                ++ii; //inc one more

                //skip past whitespace to see what kind of values
                while(ii < siz)
                {
                    if(jsonStrr[ii] != SPACE)
                    {
                        break;
                    }
                    ++ii;
                };

                if(ii > siz-1)
                {   
                    break;
                }

                startCounter =0; 
                endCounter =0;
                switch(jsonStrr[ii])
                {
                    case OPEN_CURLY:
                        startChar = OPEN_CURLY;
                        endChar= CLOSE_CURLY;
                        break;
                    case OPEN_BRACKET:
                        startChar = OPEN_BRACKET;
                        endChar = CLOSE_BRACKET;
                        break;
                    case QUOTE:
                        startChar = QUOTE;
                        endChar = QUOTE;
                        ++endCounter;
                        break;
                    default:
                        throw("unknown token");    
                }

                start =ii;
                //get to end of value
                while(ii < siz)
                {
                    if(jsonStrr[ii] == startChar)
                    {
                        ++endCounter;
                    }
                    if(jsonStrr[ii] == endChar)
                    {
                        --endCounter;
                    };
                    if(endCounter ==0)
                    {
                        break;
                    }
                    ++ii;
                };
                stop=ii;

                //rewind if we hit the end of the thing
                if(endChar != CLOSE_CURLY)
               {
                   while(jsonStrr[stop] ==CLOSE_CURLY)
                   {
                       --stop;
                    };
                };

                vallen = stop-start+1;
                this->add(key, std::move(jsonStrr.substr(start, vallen)));
            };
        }

        std::string get(const std::string &key)
        {
            return this->charMap[key];
        }

        void add(const std::string &key, const std::string &&val)
        {
            this->keyList.push_back(key);
            this->charMap[key]= std::move(val);
        }

        std::string stringify()
        {
            std::string  out; 
            int siz = this->keyList.size();
            out.reserve(this->buffSize + siz*10);
            out +=  OPEN_CURLY;
            int count =0 ;
            for(auto entry : this->keyList)
            {
                out += QUOTE;
                out += entry;
                out += QUOTE;
                out += COLIN;
                out += this->charMap[entry];

                if(count < siz-1)
                {
                    out += COMMA;
                }

                out += '\n';
                ++count;
            }  

            out += CLOSE_CURLY;
            return out;
        }

    private:
        int buffSize = 5200; //default buffer 
        std::vector<const std::string> keyList;
        std::unordered_map<std::string, std::string> charMap;
};
};

#endif