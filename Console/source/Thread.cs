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
            Console.WriteLine("Job :{0}, Exit", count);
        }
    }

    class TheradTest
    {
        public void Test()
        {
            //mothod thread
            Thread thread = new Thread(ThreadMethod);     //执行的必须是无返回值的方法
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

        public static void ThreadMethod(object parameter)  
        {
            int t = (int)parameter;

            Console.WriteLine("ThreadMethod 我是:{0},我要终止了", Thread.CurrentThread.Name);
        }

        public static void ThreadPoolMethod(object parameter)  
        {
            Job job = (Job)parameter;
            job.Run();
        }
    }
}