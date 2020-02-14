#include "extend.h"

void testSpinWait()
{
    int nIters =200; 
    int sleepTimeMicrosecs =20000; 
    u_int64_t t0; 
    double allTime =0.0; 
    for(int ii=0; ii<nIters; ii++)
    {
        t0 = extend::getTimeMicroSecs(); 
        extend::spinWait(sleepTimeMicrosecs);
        allTime += extend::getTimeMicroSecs() - t0; 
    }

    double avg = (allTime/nIters) - sleepTimeMicrosecs; 
    std::cout<<"spinWait avg accuracy: "<< avg << " microseconds" <<std::endl;
}


void testSHEM()
{
    std::cout<<"writing to shem " <<std::endl;
 
    for(int ii=0 ; ii<10; ii++)
    {
        std::string str = "test" + std::to_string(ii); 
        extend::shareFunc(true, str.c_str());
        extend::spinWait(1000000);
    }
}

int main()
{
    //testSpinWait();
    testSHEM();
    return 0; 
}