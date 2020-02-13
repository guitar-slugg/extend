#include "extend.h"

void testSpinWait()
{
    int nIters =200; 
    int sleepTimeMicrosecs =20000; 
    u_int64_t t0; 
    double allTime =0.0; 
    for(int ii=0; ii<nIters; ii++)
    {
        t0 = getTimeMicroSecs(); 
        spinWait(sleepTimeMicrosecs);
        allTime += getTimeMicroSecs() - t0; 
    }

    double avg = (allTime/nIters) - sleepTimeMicrosecs; 
    std::cout<<"spinWait avg accuracy: "<< avg << " microseconds" <<std::endl;
}

int main()
{
    testSpinWait();
    return 0; 
}