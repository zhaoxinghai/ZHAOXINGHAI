
Map<int, string> mapStudent;

1.       map�Ĺ��캯��

map���ṩ��6�����캯��������漰���ڴ��������Щ�������Թ����������������ǽ��Ӵ���һЩmap�Ĺ��췽��������Ҫ˵�µľ��ǣ�����ͨ�������·�������һ��map��

Map<int, string> mapStudent;

2.       ���ݵĲ���

�ڹ���map���������ǾͿ�����������������ˡ����ｲ���ֲ������ݵķ�����

��һ�֣���insert��������pair���ݣ��������˵��(���´�����Ȼ������д�ģ�Ӧ�ÿ�����VC��GCC�±���ͨ������ҿ��������¿�ʲôЧ������VC�������������䣬����4786����  ��pragma warning (disable:4786) )

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, ��student_one��));

       mapStudent.insert(pair<int, string>(2, ��student_two��));

       mapStudent.insert(pair<int, string>(3, ��student_three��));

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

�ڶ��֣���insert��������value_type���ݣ��������˵��

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(map<int, string>::value_type (1, ��student_one��));

       mapStudent.insert(map<int, string>::value_type (2, ��student_two��));

       mapStudent.insert(map<int, string>::value_type (3, ��student_three��));

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

�����֣������鷽ʽ�������ݣ��������˵��

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  ��student_one��;

       mapStudent[2] =  ��student_two��;

       mapStudent[3] =  ��student_three��;

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

���������÷�����Ȼ������ʵ�����ݵĲ��룬����������������ģ���Ȼ�˵�һ�ֺ͵ڶ�����Ч���������һ���ģ���insert�����������ݣ������ݵĲ������漰�����ϵ�Ψһ������������map��������ؼ���ʱ��insert�����ǲ������ݲ��˵ģ����������鷽ʽ�Ͳ�ͬ�ˣ������Ը�����ǰ�ùؼ��ֶ�Ӧ��ֵ���ó���˵��

mapStudent.insert(map<int, string>::value_type (1, ��student_one��));

mapStudent.insert(map<int, string>::value_type (1, ��student_two��));

�������������ִ�к�map��1����ؼ��ֶ�Ӧ��ֵ�ǡ�student_one�����ڶ�����䲢û����Ч����ô����漰��������ô֪��insert����Ƿ����ɹ��������ˣ�������pair������Ƿ����ɹ�����������

Pair<map<int, string>::iterator, bool> Insert_Pair;

Insert_Pair = mapStudent.insert(map<int, string>::value_type (1, ��student_one��));

����ͨ��pair�ĵڶ���������֪���Ƿ����ɹ������ĵ�һ���������ص���һ��map�ĵ��������������ɹ��Ļ�Insert_Pair.secondӦ����true�ģ�����Ϊfalse��

���������ɴ��룬��ʾ����ɹ��������

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

	   Pair<map<int, string>::iterator, bool> Insert_Pair;

       Insert_Pair �� mapStudent.insert(pair<int, string>(1, ��student_one��));

       If(Insert_Pair.second == true)

       {

              Cout<<��Insert Successfully��<<endl;

       }

       Else

       {

              Cout<<��Insert Failure��<<endl;

       }

       Insert_Pair �� mapStudent.insert(pair<int, string>(1, ��student_two��));

       If(Insert_Pair.second == true)

       {

              Cout<<��Insert Successfully��<<endl;

       }

       Else

       {

              Cout<<��Insert Failure��<<endl;

       }

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

��ҿ��������³��򣬿�����������������ݸ����ϵ�Ч��

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  ��student_one��;

       mapStudent[1] =  ��student_two��;

       mapStudent[2] =  ��student_three��;

       map<int, string>::iterator  iter;

       for(iter = mapStudent.begin(); iter != mapStudent.end(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

3.       map�Ĵ�С

����map������������ݣ�������ô֪����ǰ�Ѿ������˶��������أ�������size�������÷����£�

Int nSize = mapStudent.size();


4.       ���ݵı���

����Ҳ�ṩ���ַ�������map���б���

��һ�֣�Ӧ��ǰ���������������������е��������ˣ��Թ�����

�ڶ��֣�Ӧ�÷�����������������˵����Ҫ���Ч�������Ը��������г���

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, ��student_one��));

       mapStudent.insert(pair<int, string>(2, ��student_two��));

       mapStudent.insert(pair<int, string>(3, ��student_three��));

       map<int, string>::reverse_iterator  iter;

       for(iter = mapStudent.rbegin(); iter != mapStudent.rend(); iter++)

{

       Cout<<iter->first<<��   ��<<iter->second<<end;

}

}

�����֣������鷽ʽ������˵������

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, ��student_one��));

       mapStudent.insert(pair<int, string>(2, ��student_two��));

       mapStudent.insert(pair<int, string>(3, ��student_three��));

       int nSize = mapStudent.size()

//�˴�����Ӧ���� for(int nIndex = 1; nIndex <= nSize; nIndex++) 


//by rainfish

       for(int nIndex = 0; nIndex < nSize; nIndex++)

{

       Cout<<mapStudent[nIndex]<<end;

}

}

5.       ���ݵĲ��ң������ж�����ؼ����Ƿ���map�г��֣�

���������ǽ���ᣬmap�����ݲ���ʱ��֤����ĺô���

Ҫ�ж�һ�����ݣ��ؼ��֣��Ƿ���map�г��ֵķ����Ƚ϶࣬���������Ȼ�����ݵĲ��ң������ｫ�����Ŵ�����map�����÷���

��������������ݲ��ҷ���

��һ�֣���count�������ж��ؼ����Ƿ���֣���ȱ�����޷���λ���ݳ���λ��,����map�����ԣ�һ��һ��ӳ���ϵ���;�����count�����ķ���ֵֻ��������Ҫô��0��Ҫô��1�����ֵ��������Ȼ�Ƿ���1��

�ڶ��֣���find��������λ���ݳ���λ�ã������ص�һ���������������ݳ���ʱ����������������λ�õĵ����������map��û��Ҫ���ҵ����ݣ������صĵ���������end�������صĵ�����������˵��

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, ��student_one��));

       mapStudent.insert(pair<int, string>(2, ��student_two��));

       mapStudent.insert(pair<int, string>(3, ��student_three��));

       map<int, string>::iterator iter;

       iter = mapStudent.find(1);

if(iter != mapStudent.end())

{

       Cout<<��Find, the value is ��<<iter->second<<endl;

}

Else

{

       Cout<<��Do not Find��<<endl;

}

}

�����֣�������������ж������Ƿ���֣����Եñ��˵㣬���ǣ��Ҵ��������ｲ��

Lower_bound�����÷������������������Ҫ���ҹؼ��ֵ��½�(��һ��������)

Upper_bound�����÷������������������Ҫ���ҹؼ��ֵ��Ͻ�(��һ��������)

���磺map���Ѿ�������1��2��3��4�Ļ������lower_bound(2)�Ļ������ص�2����upper-bound��2���Ļ������صľ���3

Equal_range��������һ��pair��pair�����һ��������Lower_bound���صĵ�������pair����ڶ�����������Upper_bound���صĵ������������������������ȵĻ�����˵��map�в���������ؼ��֣�����˵��

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent[1] =  ��student_one��;

       mapStudent[3] =  ��student_three��;

       mapStudent[5] =  ��student_five��;

       map<int, string>::iterator  iter;

iter = mapStudent.lower_bound(2);

{

       //���ص����½�3�ĵ�����

       Cout<<iter->second<<endl;

}

iter = mapStudent.lower_bound(3);

{

       //���ص����½�3�ĵ�����

       Cout<<iter->second<<endl;

}

 

iter = mapStudent.upper_bound(2);

{

       //���ص����Ͻ�3�ĵ�����

       Cout<<iter->second<<endl;

}

iter = mapStudent.upper_bound(3);

{

       //���ص����Ͻ�5�ĵ�����

       Cout<<iter->second<<endl;

}

 

Pair<map<int, string>::iterator, map<int, string>::iterator> mapPair;

mapPair = mapStudent.equal_range(2);

if(mapPair.first == mapPair.second)
       {

       cout<<��Do not Find��<<endl;

}

Else

{

Cout<<��Find��<<endl;
}

mapPair = mapStudent.equal_range(3);

if(mapPair.first == mapPair.second)
       {

       cout<<��Do not Find��<<endl;

}

Else

{

Cout<<��Find��<<endl;
}

}

6.       ���ݵ�������п�

���map�е����ݿ�����clear()�������ж�map���Ƿ������ݿ�����empty()������������true��˵���ǿ�map

7.       ���ݵ�ɾ��

����Ҫ�õ�erase�������������������˵ĺ�������������������ϸ˵�����ǵ��÷�

#include <map>

#include <string>

#include <iostream>

Using namespace std;

Int main()

{

       Map<int, string> mapStudent;

       mapStudent.insert(pair<int, string>(1, ��student_one��));

       mapStudent.insert(pair<int, string>(2, ��student_two��));

       mapStudent.insert(pair<int, string>(3, ��student_three��));

 

//�����Ҫ��ʾ���Ч������ѡ�����µ�һ�֣��㿴����Ч����ȽϺ�

       //���Ҫɾ��1,�õ�����ɾ��

       map<int, string>::iterator iter;

       iter = mapStudent.find(1);

       mapStudent.erase(iter);

 

       //���Ҫɾ��1���ùؼ���ɾ��

       Int n = mapStudent.erase(1);//���ɾ���˻᷵��1�����򷵻�0

 

       //�õ���������Ƭ��ɾ��

       //һ�´��������map���

       mapStudent.earse(mapStudent.begin(), mapStudent.end());

       //��Ƭɾ��Ҫע����ǣ�Ҳ��STL�����ԣ�ɾ��������һ��ǰ�պ󿪵ļ���

 

       //�Ը����ϱ������룬��ӡ�����

}

8.       ����һЩ�����÷�

������swap,key_comp,value_comp,get_allocator�Ⱥ������о�����Щ�����ڱ���õĲ��Ǻܶ࣬�Թ�����������Ȥ�Ļ������Ը��о�

9.       ����

����Ҫ������һ��Ƚϸ�����÷���,�������⣬STL��Ĭ���ǲ���С�ں�������ģ����ϴ������������ǲ������κ�����ģ���Ϊ����Ĺؼ�����int�ͣ�������֧��С�ں����㣬��һЩ�������������ؼ�����һ���ṹ�壬�漰������ͻ�������⣬��Ϊ��û��С�ںŲ�����insert�Ⱥ����ڱ����ʱ�����ȥ���������������������������

��һ�֣�С�ں����أ��������

#include <map>

#include <string>

Using namespace std;

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

}StudentInfo, *PStudentInfo;  //ѧ����Ϣ

 

Int main()

{

    int nSize;

       //��ѧ����Ϣӳ�����

       map<StudentInfo, int>mapStudent;

    map<StudentInfo, int>::iterator iter;

       StudentInfo studentInfo;

       studentInfo.nID = 1;

       studentInfo.strName = ��student_one��;

       mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));

       studentInfo.nID = 2;

       studentInfo.strName = ��student_two��;

mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));

 

for (iter=mapStudent.begin(); iter!=mapStudent.end(); iter++)

    cout<<iter->first.nID<<endl<<iter->first.strName<<endl<<iter->second<<endl;

 

}

���ϳ������޷�����ͨ���ģ�ֻҪ����С�ںţ���OK�ˣ����£�

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

       Bool operator < (tagStudentInfo const& _A) const

       {

              //�������ָ��������ԣ���nID�������nID��ȵĻ�����strName����

              If(nID < _A.nID)  return true;

              If(nID == _A.nID) return strName.compare(_A.strName) < 0;

              Return false;

       }

}StudentInfo, *PStudentInfo;  //ѧ����Ϣ

�ڶ��֣��º�����Ӧ�ã����ʱ��ṹ����û��ֱ�ӵ�С�ں����أ�����˵��

#include <map>

#include <string>

Using namespace std;

Typedef struct tagStudentInfo

{

       Int      nID;

       String   strName;

}StudentInfo, *PStudentInfo;  //ѧ����Ϣ

 

Classs sort

{

       Public:

       Bool operator() (StudentInfo const &_A, StudentInfo const &_B) const

       {

              If(_A.nID < _B.nID) return true;

              If(_A.nID == _B.nID) return _A.strName.compare(_B.strName) < 0;

              Return false;

       }

};

 

Int main()

{

       //��ѧ����Ϣӳ�����

       Map<StudentInfo, int, sort>mapStudent;

       StudentInfo studentInfo;

       studentInfo.nID = 1;

       studentInfo.strName = ��student_one��;

       mapStudent.insert(pair<StudentInfo, int>(studentInfo, 90));

       studentInfo.nID = 2;

       studentInfo.strName = ��student_two��;

mapStudent.insert(pair<StudentInfo, int>(studentInfo, 80));

}

10.   ����

����STL��һ��ͳһ�����壬map�ĺܶ��÷�����STL�������Ķ��������һ�𣬱����������ϣ�����Ĭ���õ���С�ںţ���less<>�����Ҫ�Ӵ�С�����أ������漰���Ķ����ܶ࣬�ڴ��޷�һһ����˵����

��Ҫ˵�����ǣ�map���������ڲ������ɺ������֤����˺ܶຯ��ִ�е�ʱ�临�Ӷȶ���log2N�ģ������map��������ʵ�ֵĹ��ܣ���STL  AlgorithmҲ������ɸù��ܣ�������map�Դ�������Ч�ʸ�һЩ��

����˵�£�map�ڿռ��ϵ����ԣ����򣬹���������������ʱ����ֵıȽ����ƣ�����map��ÿ�����ݶ�Ӧ������ϵ�һ���ڵ㣬����ڵ��ڲ������������ʱ����ռ��16���ֽڵģ�һ�����ڵ�ָ�룬���Һ���ָ�룬����һ��ö��ֵ����ʾ��ڵģ��൱��ƽ��������е�ƽ�����ӣ���������Ӧ��֪������Щ�ط��ܷ��ڴ��˰ɣ���˵�ˡ���
