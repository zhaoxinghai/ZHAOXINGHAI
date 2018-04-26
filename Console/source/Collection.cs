using System;
using System.Collections.Generic;

namespace _1Person
{
    class CItem
    {
        public CItem(int nID,String sName)
        {
            ID = nID;
            strName = sName;
        }
        public int ID;
        public String strName;
    }
    class CollectionTest
    {
        //List
        public void test()
        {
            //collections
            List<String> list = new List<string>();
            List<CItem> list2 = new List<CItem>();
            LinkedList<String> linkedList = new LinkedList<String>();
            Dictionary<Int32, String> dic = new Dictionary<Int32, String>();

            //easy type use this
            HashSet<String>  set = new HashSet<String>();
        
            //Add
            for (int i = 0; i < 10; ++i )
            {
                list.Add(i.ToString());
                list.Add(i.ToString());

                linkedList.AddLast(i.ToString());
                dic.Add(i, i.ToString());
                set.Add(i.ToString());

                //extension
                list2.Add(new CItem(i,"test"));
            }

            //Read
            Console.WriteLine("List: ");
            foreach (var item in list)
            {
                Console.WriteLine(item);
            }

            Console.WriteLine("\nLinkedList: ");
            foreach (var item in linkedList)
            {
                Console.WriteLine(item);
            }

            Console.WriteLine("\nDictionary: ");
            foreach (var item in dic)
            {
                Console.WriteLine("Key = {0}, Value = {1}", item.Key, item.Value);
            }

            Console.WriteLine("\nHashSet: ");
            foreach (var item in set)
            {
                Console.WriteLine(item);
            }

            //List remove
            list.Remove("1");  //only delete one item
        
            //List 2 remove all(reverse remove)
            for (int i = list2.Count-1; i>=0; i--)
            {
                if (list2[i].ID == i)
                {
                    list2.RemoveAt(i);
                }
            }

            //LinkedListNode remove by iter
            LinkedListNode<String> now = linkedList.First;
            while(now != linkedList.Last)
            {
                if(now.Value=="test")
                {
                    LinkedListNode<String> tmp = now.Next;
                    linkedList.Remove(now);
                    now = tmp;
                }
                else
                {
                    now = now.Next;
                }
            }

            //dictionary remove
            dic.Remove(1);
            set.Remove("1");
        }
    }
}