using System;
using System.Threading;
using System.Text;

namespace _1Person
{
    class Job
    {
        public int count = 0;
        public string name = "zhaoxinghai";

        public void Run()
        {
            Thread.Sleep(2000);
            System.Console.WriteLine("Job :{0}, Exit", count);
        }
    }

    class TheradTest
    {
        private Object locker = new Object();

        private Semaphore sema = new Semaphore(0,5);

        public static Mutex mutex = new Mutex();

        //autoEvent.set(),autoEvent.WaitOne();
        static AutoResetEvent autoEvent = new AutoResetEvent(false);  

        //manualEvent.set(),manualEvent.reset(),manualEvent.WaitOne();
        static ManualResetEvent manualEvent = new ManualResetEvent(false);

        public void Test()
        {
            lock(locker)
            {
                //mothod thread
                Thread thread = new Thread(ThreadMethod);
                //thread.Start(54);

                //member thread
                var job = new Job();
                Thread th = new Thread(job.Run);
                th.Start();

                //threadpool
                ThreadPool.SetMaxThreads(5, 5);
                for(int i = 0;i<100;i++)
                {
                    job = new Job();
                    job.count = i;
                    ThreadPool.QueueUserWorkItem(ThreadPoolMethod,job);
                }
            }
        }

        public static void ThreadMethod(object parameter)  
        {
            mutex.WaitOne();

            int t = (int)parameter;

            System.Console.WriteLine("ThreadMethod 我是:{0},我要终止了", Thread.CurrentThread.Name);

            mutex.ReleaseMutex();
        }

        public static void ThreadPoolMethod(object parameter)  
        {
            Job job = (Job)parameter;
            job.Run();
        }


        private static Mutex m;
        public static bool IsSingleInstance()
        {
            //是否需要创建一个应用
            Boolean isCreateNew = false;
            try
            {
               m = new Mutex(initiallyOwned: true, name: "SingleInstanceMutex", createdNew: out isCreateNew);
            }
            catch (Exception ex)
            {
                System.Console.WriteLine(ex.ToString());
            }
            return isCreateNew;
        }
    }
}