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
            var net = new MySocket();
            net.Test();

            //file system
            var file = new FileSystem();
            file.Test();

            Thread.Sleep(1000000);
        }
    }
}
