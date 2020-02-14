using System;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace wrapCS
{
    class Program
    {
        private const string LIB_PATH ="../lib/libextend.dylib"; 

        static void Main(string[] args)
        {
            hello();

            int nIters = 500000; 

            Stopwatch watch = new Stopwatch();
            watch.Start();
            for(int ii =0; ii< nIters; ii++)
            {
                bareWrapper();
            }
            long elapsed = watch.ElapsedMilliseconds; 
            double wrapperTimeMicro = ((double)elapsed/nIters)*1000;
            Console.WriteLine("Wrapper overhead micosecs:" + wrapperTimeMicro );
        }

        //be sure the C/++ code declares extern "C" of the same signature 
        [DllImport(LIB_PATH)]
        public static extern void hello();   


        [DllImport(LIB_PATH)]
        public static extern void bareWrapper();  

    }
}
