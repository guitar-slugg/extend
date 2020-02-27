#ifndef JSON_H
#define JSON_H

#include <iostream>
#include <vector>
#include <stdlib.h>
#include "utils.h"

//light and fast JSON serializer and parser

namespace extend
{   
    class JsonObject
    {
        public:
            JsonObject()
            {
                this->buffer[0] = '{'; this->index = 1;
            }

            const char * toJson()
            {
                if(this->buffer[this->index-1] == ',')
                {
                    this->buffer[this->index-1] = '}';
                    this->buffer[this->index] = '\0';
                    return this->buffer; 
                };

                this->buffer[this->index] = '}'; ++this->index;
                return this->buffer;
            }

            void add(const char * key, int value)
            {
                putKey(key);
                sprintf(this->numBuffer, "%d", value);
                putNumBuff();
            }

            void add(const char * key, double value)
            {
                putKey(key);
                sprintf(this->numBuffer, "%f", value);
                putNumBuff();
            }

            void add(const char * key, const char * value)
            {
                putKey(key);
                putCharBuff(value, true);
            }

            void add(const char * key, const std::string value)
            {
                add(key, value.c_str());
            }

            void add(const char * key, bool value)
            {
                putKey(key);
                if(value)
                {
                    putCharBuff("true", false);
                }else
                {
                    putCharBuff("false", false);
                }
            }

            void add(const char * key, JsonObject &objj)
            {
                putKey(key);
                putCharBuff(objj.toJson(),false);
            }

            void addNull(const char * key)
            {
                putKey(key);
                putCharBuff("null", false);
            }

            template <class T>
            void addNumericArray(const char * key, const std::vector<T> &vect, bool floatingPoint)
            {
                putKey(key);
                this->buffer[this->index] = '['; ++this->index;
                const char * type; 
                if(floatingPoint)
                {
                    type = "%f";
                } else
                {
                    type = "%d";
                }
                
                for(int jj=0; jj <vect.size(); jj++)
                {
                    sprintf(this->numBuffer, type, vect.at(jj));
                    putNumBuff();
                }
                --this->index;
                this->buffer[this->index] = ']'; ++this->index;
                this->buffer[this->index] = ',';++this->index;
            }

            void addCharArray(const char * key, const std::vector<const char *> &vect)
            {
                putKey(key);
                this->buffer[this->index] = '['; ++this->index;
                const char * type;                 
                for(int jj=0; jj <vect.size(); jj++)
                {
                    putCharBuff(vect.at(jj), true);
                }
                --this->index;
                this->buffer[this->index] = ']'; ++this->index;
                this->buffer[this->index] = ',';++this->index;
            }

            void addBoolArray(const char * key, const std::vector<bool> &vect)
            {
                putKey(key);
                this->buffer[this->index] = '['; ++this->index;
                const char * type;                 
                for(int jj=0; jj <vect.size(); jj++)
                {
                    if(vect.at(jj))
                    {
                        putCharBuff("true", false);
                    }else{
                        putCharBuff("false", false);
                    }
                }
                --this->index;
                this->buffer[this->index] = ']'; ++this->index;
                this->buffer[this->index] = ',';++this->index;
            }

            void addObjectArray(const char * key, std::vector<JsonObject> &vect)
            {
                putKey(key);
                this->buffer[this->index] = '['; ++this->index;              
                for(int jj=0; jj <vect.size(); jj++)
                {
                    putCharBuff(vect.at(jj).toJson(), false);
                }
                --this->index;
                this->buffer[this->index] = ']'; ++this->index;
                this->buffer[this->index] = ',';++this->index;
            };

            void addRaw(const char * strr)
            {
                int ii; 
                while(strr[ii] != '\0')
                {
                    this->buffer[this->index] = strr[ii]; this->index++; ii++;
                }
            };

            int getLen()
            {
                return this->index +1;
            }

            void clear()
            {
                int ii; 
                while(this->buffer[ii] != '\0')
                {
                    this->buffer[ii] ='\0';
                    ++ii;
                }
                this->buffer[0] = '{'; this->index = 1;
            }

            const char * findVal(const char * key, const char * jsonBuffer )
            {
                int startVal =1;
                int stopVal =0;
                int tempLen;
                while(jsonBuffer[startVal]!='\0')
                {
                    //skip over "
                    while(jsonBuffer[startVal]=='"' && jsonBuffer[startVal]!='\0')
                    {
                        ++startVal;
                    }

                    //get to the end of the key
                    stopVal = startVal;
                    while(jsonBuffer[stopVal]!='"')
                    {
                        ++stopVal;
                    }

                    tempLen= stopVal-startVal;
                    memcpy(&keyBuffer, &jsonBuffer[startVal], tempLen);
                    keyBuffer[tempLen] = '\0';
                    startVal = stopVal +1;

                    //check if this key is good
                    if(strcmp(keyBuffer,key)==0)
                    {
                        //skip over :
                        while(jsonBuffer[startVal]!=':' && jsonBuffer[startVal]!='\0')
                        {
                            startVal++;
                        }
                        startVal++;

                        //get to end of value
                        while((jsonBuffer[stopVal]!=',' && jsonBuffer[stopVal]!='}')  && jsonBuffer[stopVal]!='\0')
                        {
                            ++stopVal;
                        }

                        tempLen= stopVal-startVal;
                        memcpy(&keyBuffer, &jsonBuffer[startVal], tempLen);
                        keyBuffer[tempLen] = '\0';
                        return keyBuffer;
                    }else
                    {
                        //skip over un needed value
                        while(jsonBuffer[startVal]!='"' && jsonBuffer[startVal]!='\0')
                        {
                            startVal++;
                        }

                    }
                    
                }

                return "NOT_FOUND";
            }

        private:
            static const int BUFFLEN = 5200;
            char buffer[BUFFLEN];
            char numBuffer[100];
            char keyBuffer[512];
            int index;

            void putKey(const char * key)
            {
               this->buffer[this->index] = '"'; ++this->index;

                for(int ii =0; ii<strlen(key); ii++)
                {
                    this->buffer[index] = key[ii];
                    ++this->index;
                }

                this->buffer[this->index] = '"'; ++this->index;
                this->buffer[this->index] = ':'; ++this->index;
            };

            void putNumBuff()
            {
                for( int ii =0; ii<strlen(this->numBuffer); ii++)
                {
                    this->buffer[index] = this->numBuffer[ii];
                    ++this->index;
                }
                this->buffer[this->index] = ',';
                ++this->index;
            }

            void putCharBuff(const char * value, bool stringify)
            {
                if(stringify)
                {
                    this->buffer[index] = '"'; ++this->index;
                }

                for(int ii =0; ii<strlen(value); ii++)
                {
                    this->buffer[index] = value[ii];
                    ++this->index;
                }

                if(stringify)
                {
                    this->buffer[index] = '"'; ++this->index;
                }

                this->buffer[this->index] = ',';++this->index;
            }

    };
}

#endif