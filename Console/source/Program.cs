using System;
using System.Threading;

namespace _1Person
{
    class Program
    {
        static void Main(string[] args)
        {
            //test Collection
            var col = new CollectionTest();
            col.test();

            //test thread
            var thr = new TheradTest();
            thr.Test();

            //test socket
            var soc = new Socket();
            soc.Test();

            Thread.Sleep(1000000);
        }
    }
}
