#ifndef RING_H
#define RING_H

#include<queue>

namespace extend
{

template <class T>
class RingBuffer
{
    public:
        RingBuffer(int size)
        {
            this->size = size;
        }

        void push(const T &val)
        {
            std::lock_guard<std::mutex> guard(this->mut);
            if(this->nObjs >= this->size)
            {
                --this->nObjs; 
                this->q.pop();
            }
            this->q.push(val);
            ++this->nObjs; 
        }

        T pop()
        {
            std::lock_guard<std::mutex> guard(this->mut);
            --this->nObjs;
            T obj = this->q.front();
            this->q.pop();
            return obj;
        }

        int available()
        {
            return this->nObjs;
        }

    private:
        int size; 
        int nObjs =0;
        std::queue<T> q;
        std::mutex mut;
};

}
#endif