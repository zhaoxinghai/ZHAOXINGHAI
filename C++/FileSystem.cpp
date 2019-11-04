using System;
using System.IO;
using System.Text;

namespace _1Person
{
    class FileSystem
    {
        public void Test()
        { 
            Write();

            Read();
        }

        public void Read()
        {
            try
            {
                string strDir = System.Environment.CurrentDirectory;
                FileStream file = new FileStream(strDir + "\\test.txt", FileMode.Open);
                file.Seek(0, SeekOrigin.Begin);

                byte[] byData = new byte[100];
                file.Read(byData, 0, 100); 
                
                string str = System.Text.Encoding.Default.GetString ( byData );
                
                file.Close();
            }
            catch (IOException e)
            {
                System.Console.WriteLine(e.ToString());
            }
        }


        public void Write()
        {
            string strDir = System.Environment.CurrentDirectory;
            FileStream fs = new FileStream(strDir + "\\test.txt", FileMode.Create);
           
            byte[] data = System.Text.Encoding.Default.GetBytes("Hello World!"); 
            fs.Write(data, 0, data.Length);
         
            fs.Flush();
            fs.Close();
        }
    }
}