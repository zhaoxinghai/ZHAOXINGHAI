using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

//C#的窗口测试
namespace Console
{
    
    public class LIST_ITEM
    {
        public int nID { get; set; }
        public string sName { get; set; }
    }
    
    public class GenericT
    {
        public List<LIST_ITEM> m_List = new List<LIST_ITEM>();

        public LinkedList<LIST_ITEM> m_Linked = new LinkedList<LIST_ITEM>();

        public Dictionary<string, LIST_ITEM> m_Dictionary = new Dictionary<string, LIST_ITEM>();

        public Queue<LIST_ITEM> m_Queue = new Queue<LIST_ITEM>();

        //购造函数没不用返回类型
        public GenericT()
        {
            //List();
            //Linked();
            //Dictionary();
        }
        //静态函数一定要包括在类里，不能在名称空间定义
        public static int compare(LIST_ITEM item1, LIST_ITEM item2)
        {
            if (item1.nID > item2.nID)
            {
                return 1;
            }
            else
            {
                return -1;
            }
        }

        public void List()
        {
            //增加数据
            for (int i = 0; i < 10; i++)
            {
                LIST_ITEM item = new LIST_ITEM();
                item.nID = i;
                item.sName = "zhao";
                m_List.Add(item);
            }
            //取值与修改
            LIST_ITEM val = m_List.ElementAt(0);
            val = m_List[0];
            val.nID = 888;

            //删除索引1号元素
            m_List.RemoveAt(1);

            //枚举
            foreach(LIST_ITEM item in m_List)
            {
                System.Console.Write(item.nID);
                System.Console.Write("\n");
            }
            //排序
            m_List.Sort(compare);
        }
        public void Linked()
        {
            //增加数据AddBefore()/AddAfter()/AddFirst()/AddLast()
            for (int i = 0; i < 10; i++)
            {
                LIST_ITEM item = new LIST_ITEM();
                item.nID = i;
                item.sName = "zhao";
                m_Linked.AddLast(item);
            }
            //取值与修改
            LIST_ITEM val = m_Linked.First();
            val.nID = 888;

            //删除元素
            LinkedListNode<LIST_ITEM> NowNode = m_Linked.First;
            LinkedListNode<LIST_ITEM> lastNode = m_Linked.Last;
            while(NowNode != lastNode)
            {
                if (NowNode.Value.nID == 3)
                {
                    LinkedListNode<LIST_ITEM> tmpNode = NowNode.Next;
                    m_Linked.Remove(NowNode);
                    NowNode = tmpNode;
                }
                else
                {
                    NowNode = NowNode.Next;
                }
            }
            
            //枚举
            foreach (LIST_ITEM item in m_Linked)
            {
                System.Console.Write(item.nID);
                System.Console.Write("\n");
            }
        }
        public void Dictionary()
        {
            //插入数据1
            LIST_ITEM val = new LIST_ITEM();
            m_Dictionary.Add("zhao", val);
            
            //插入数据2(推荐)
            m_Dictionary["xing"] = val;

            //删除
            m_Dictionary.Remove("zhao");
            //m_Dictionary.Clear();

            //枚举
            foreach (KeyValuePair<string,LIST_ITEM> pair in m_Dictionary)
            {
                System.Console.WriteLine("Key:{0}, Value:{1}", pair.Key, pair.Value);
            }

            //遍历keys
            foreach (string key in m_Dictionary.Keys)
            {

            }
            //遍历values
            foreach(LIST_ITEM value in m_Dictionary.Values)
            {

            }
            //读取数据
            if(m_Dictionary.ContainsKey("zhao"))
            {
                LIST_ITEM vTest = m_Dictionary["zhao"];
            }  
        }
        public void Queue()
        {
            LIST_ITEM val = new LIST_ITEM();

            //插入队列
            m_Queue.Enqueue(val);

            //取出最先入队的元素，但并不删除
            val = m_Queue.Peek();

            //弹出队列(删除)
            val = m_Queue.Dequeue();

            //枚举
            foreach (LIST_ITEM value in m_Queue)
            {
                System.Console.Write(value.sName);
                System.Console.Write("\n");
            }
        }
    }
}
