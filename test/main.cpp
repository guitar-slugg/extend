#include "simlib.h"

void testSpinWait()
{
    int nIters =200; 
    int sleepTime =20; 
    u_int64_t t0; 
    double allTime =0.0; 
    for(int ii=0; ii<nIters; ii++)
    {
        t0 = getTimeMs(); 
        spinWait(sleepTime);
        allTime += getTimeMs() - t0; 
    }

    double drift = (allTime/nIters)/sleepTime -1 ; 
    std::cout<<"spin wait drift: "<< drift <<std::endl;
}

int main()
{
    testSpinWait();
    return 0; 
}