using System;
using System.Net;
using System.Net.Sockets;
using System.Threading;

namespace _1Person
{
    class MySocket
    {
        public void Test()
        {
            AcceptTest();

            Thread.Sleep(1000*3);
            Connect();
        }

        public void AcceptTest()
        {
            Socket socketWatch = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);
          
            IPAddress ip = IPAddress.Parse("127.0.0.1");
            
            IPEndPoint point = new IPEndPoint(ip,Convert.ToInt32(1000));
           
            socketWatch.Bind(point);
      
            socketWatch.Listen(10);

            Thread thread = new Thread(Accept);
            thread.Start(socketWatch);
        }

        public static void Accept(object parameter)
        {
            Socket socket = (Socket)parameter;
            Socket socketAccept = socket.Accept();

            while(true)
            {
                byte[] data = System.Text.Encoding.Default.GetBytes("Hello World!"); 
                string str = System.Text.Encoding.Default.GetString ( data );

                socketAccept.Send(data);

                Thread.Sleep(1000);
            }
        }
        public void Connect()
        {
            Socket socketSend = new Socket(AddressFamily.InterNetwork, SocketType.Stream, ProtocolType.Tcp);

            IPAddress ip = IPAddress.Parse("127.0.0.1");
                  
            socketSend.Connect(ip, Convert.ToInt32("1000"));

            int count = 0;

            while(true)
            {
                byte[] buffer = new byte[2048];
                int r = socketSend.Receive(buffer);

                if(r > 0)
                {
                    byte[] text = new byte[r];
                    Array.Copy(buffer, text, r);
                    string str = System.Text.Encoding.Default.GetString ( text );
                    System.Console.WriteLine("{0}",str);
                }

                count += r;
                if(count >= 1)
                {
                    break;
                }
            }
        }
    }
}