

//1 ����һ��list

#include <string>
#include <list>

list<string> Milkshakes;

//��Ա����
void push_back(const _Ty& _Val);
void push_front(const _Ty& _Val);
void pop_back();
void pop_front();
iterator erase(const_iterator _Where);
bool empty()


//����
list<string> list;
list<string>::iterator it;
for (it=list.begin();it!=list.end();it++)
{
    cout << (*it) << endl;
}

//���Һ���
it = find (Fruit.begin(), Fruit.end(), "Pineapple");

    if (FruitIterator == Fruit.end())

    {

        cout << "Fruit not found in list" << endl;

    }

    else

    {

        cout << *FruitIterator << endl;

    }

}

10.ʹ��STLͨ���㷨search��list����һ������

 

һЩ�ַ���STL�����кܺô��������ǿ�һ��һ���Ѵ�����ַ����С����ǽ�����һ��list�����ַ���

list<char> Characters;

 

������������һ���ַ����У��������κΰ�����֪��Ȼ������ڴ档��֪�����Ǵ����￪ʼ������������� ���ǳ����á��Ҳ�֪�����Ƿ�˵����null��β���ַ����顣

 

�����Ǽ���һЩ����ϲ�����ַ������list�У�

 

Characters.push_back('\0');

Characters.push_back('\0');

Characters.push_back('1');

Characters.push_back('2');

���ǽ��õ����ٸ����ַ��أ�

int NumberOfNullCharacters(0);

count(Characters.begin(), Characters.end(), '\0', NumberOfNullCharacters);

cout << "We have " << NumberOfNullCharacters << endl;

���������ַ�'1'

list<char>::iterator Iter;

Iter = find(Characters.begin(), Characters.end(), '1');

cout << "We found " << *Iter << endl;

���������ʾ��STL�����������Ը���׼�ķ�����������ַ���������������STL��search�㷨������������ ������null��������µ�һ����STLͨ���㷨search()��������һ������������������һ��Ԫ�ش�������find()��find_if() ֻ����������Ԫ�ء�

/*

|| How to use the search algorithm in an STL list

*/

#include <string>

#include <list>

#include <algorithm>

 

int main ( void)

{

    list<char> TargetCharacters;

    list<char> ListOfCharacters;

 

    TargetCharacters.push_back('\0');

    TargetCharacters.push_back('\0');

 

    ListOfCharacters.push_back('1');

    ListOfCharacters.push_back('2');

    ListOfCharacters.push_back('\0');

    ListOfCharacters.push_back('\0');

 

    list<char>::iterator PositionOfNulls = search(ListOfCharacters.begin(), ListOfCharacters.end(), TargetCharacters.begin(), TargetCharacters.end());

 

    if (PositionOfNulls!=ListOfCharacters.end())

        cout << "We found the nulls" << endl;

}

The output of the program will be ���ǳ���������

We found the nulls

 

search�㷨��һ������������һ�����еĵ�һ�γ��ֵ�λ�á������������������ListOfCharacters�� ��TargetCharacters������еĵ�һ�γ��֣�TargetCharacters�ǰ�������null�ַ������С� search�Ĳ���������ָ�Ų���Ŀ���iterator������ָ��������Χ��iterators�� �������������������ListOfCharacters�ķ�Χ�ڲ���TargetCharacters���list���������С�

 

���TargetCharacters�����֣�search�ͻ᷵��һ��ָ��ListOfCharacters������ƥ��ĵ�һ�� �ַ���iterator�����û���ҵ�ƥ���search����ListOfCharacters.end()��ֵ��

 

 

--------------------------------------------------------------------------------

 

11 ʹ��list�ĳ�Ա����sort()����һ��list��

 

    Ҫ����һ��list������Ҫ��list�ĳ�Ա����sort()��������ͨ���㷨sort()�����������ù����㷨����ͨ���㷨��Ȼ������STL����ʱ����֧�����Լ���һ�������㷨��ʵ�֣���ͨ����Ϊ��������ܡ�

    ����������У�list���������Լ���sort�㷨��������Ϊͨ���㷨����Ϊ��Щ�ṩ�����ȡ����Ԫ�ص��������򣬶�����list����Ϊһ�����ӵ�����ʵ�ֵģ�����֧�ֶ��������Ԫ�������ȡ�����Ծ���Ҫһ������� sort()��Ա����������list��

    ���ڸ���ԭ��������������Ҫ�ϸ߻�������Ч������ĳ���֧���ⲿ����(extra functions)�� ��ͨ�����ù��캯���Ľṹ���Կ���������

 

/* || How to sort an STL list */

#include <string>

#include <list>

#include <algorithm>

PrintIt (string& StringToPrint)

{

cout << StringToPrint << endl;

}

 

int main (void)

{

    list<string> Staff;

    list<string>::iterator PeopleIterator;

    Staff.push_back("John");

    Staff.push_back("Bill");

    Staff.push_back("Tony");

    Staff.push_back("Fidel");

    Staff.push_back("Nelson");

    cout << "The unsorted list " << endl;

    for_each(Staff.begin(), Staff.end(), PrintIt );

    Staff.sort();

    cout << "The sorted list " << endl;

    for_each(Staff.begin(), Staff.end(), PrintIt);

}

 

����ǣ�

The unsorted list  John Bill Tony Fidel Nelson The sorted list  Bill Fidel John Nelson Tony

 

 

--------------------------------------------------------------------------------

 

12.��list�ĳ�Ա��������Ԫ�ص�list��

 

    list�ĳ�Ա����push_front()��push_back()�ֱ��Ԫ�ؼ��뵽list��ǰ��ͺ��档�����ʹ��insert() �Ѷ�����뵽list�е��κεط���

    insert()���Լ���һ������һ����������ɷݿ���������һ����Χ���ڵĶ���������һЩ�������list�е����ӣ�

 

/*

|| Using insert to insert elements into a list.

*/

#include <list>

 

int main (void)

{

    list<int> list1;

 

    /*

    || Put integers 0 to 9 in the list

    */

    for (int i = 0; i < 10; ++i) list1.push_back(i);

 

    /*

    || Insert -1 using the insert member function

    || Our list will contain -1,0,1,2,3,4,5,6,7,8,9

    */

    list1.insert(list1.begin(), -1);

 

    /*

    || Insert an element at the end using insert

    || Our list will contain -1,0,1,2,3,4,5,6,7,8,9,10

    */

    list1.insert(list1.end(), 10);

 

    /*

    || Inserting a range from another container

    || Our list will contain -1,0,1,2,3,4,5,6,7,8,9,10,11,12

    */

    int IntArray[2] = {11,12};

    list1.insert(list1.end(), &IntArray[0], &IntArray[2]);

 

    /*

    || As an exercise put the code in here to print the lists!

    || Hint: use PrintIt and accept an interger

    */

}

ע�⣬insert()������һ�������ɸ�Ԫ�ز��뵽��ָ����iterator��λ�á����Ԫ�ؽ������� iteratorָ����λ����ǰ��

 

 

--------------------------------------------------------------------------------

 

13.List ���캯��

 

�����Ѿ�������������list��

list<int> Fred;

 

��Ҳ��������������һ��list����ͬʱ��ʼ������Ԫ�أ�

 

// define a list of 10 elements and initialise them all to 0

list<int> Fred(10, 0);

// list now contains 0,0,0,0,0,0,0,0,0,0

��������Զ���һ��list������һ��STL������һ����Χ����ʼ���������STL������һ����һ��list�� ������Ҫ��Ԫ��������ͬ�ĵ������Ϳ��ԡ�

vector<int> Harry;

Harry.push_back(1);

Harry.push_back(2);

#

// define a list and initialise it with the elements in Harry

list<int> Bill(Harry.begin(), Harry.end());

// Bill now contains 1,2

 

 

--------------------------------------------------------------------------------

 

14.ʹ��list��Ա������list��ɾ��Ԫ��

 

    list��Ա����pop_front()ɾ��list�еĵ�һ��Ԫ�أ�pop_back()ɾ�����һ��Ԫ�ء�����erase()ɾ����һ��iteratorָ����Ԫ�ء�������һ��erase()��������ɾ��һ����Χ��Ԫ�ء�

 

/* || Erasing objects from a list */

#include <list>

int main (void)

{

    list<int> list1;    // define a list of integers

 

    /* || Put some numbers in the list || It now contains 0,1,2,3,4,5,6,7,8,9 */

    for (int i = 0; i < 10; ++i)

        list1.push_back(i);

 

list1.pop_front();    // erase the first element 0

 

    list1.pop_back();     // erase the last element 9

 

    list1.erase(list1.begin());

    // erase the first element (1) using an iterator

 

    list1.erase(list1.begin(), list1.end());

    // erase all the remaining elements

 

    cout << "list contains " << list1.size() << " elements" << endl;

}

 

����ǣ�

list contains 0 elements

 

 

--------------------------------------------------------------------------------

 

15.��list��Ա����remove()��list��ɾ��Ԫ�ء�

 

/* || Using the list member function remove to remove elements */

#include <string>

#include <list>

#include <algorithm>

PrintIt (const string& StringToPrint)

{

cout << StringToPrint << endl;

}

 

int main (void)

{

list<string> Birds;

Birds.push_back("cockatoo");

Birds.push_back("galah");

Birds.push_back("cockatoo");

Birds.push_back("rosella");

Birds.push_back("corella");

cout << "Original list with cockatoos" << endl;

for_each(Birds.begin(), Birds.end(), PrintIt);

Birds.remove("cockatoo");

cout << "Now no cockatoos" << endl;

for_each(Birds.begin(), Birds.end(), PrintIt);

}

 

����ǣ�

  Original list with cockatoos cockatoo galah cockatoo rosella corella Now no cockatoos galah rosella corella

 

 

--------------------------------------------------------------------------------

 

16.ʹ��STLͨ���㷨remove()��list��ɾ��Ԫ��

 

ͨ���㷨remove()ʹ�ú�list�ĳ�Ա������ͬ�ķ�ʽ������һ������²��ı������Ĵ�С��

/*

|| Using the generic remove algorithm to remove list elements

*/

#include <string>

#include <list>

#include <algorithm>

 

PrintIt(string& AString) { cout << AString << endl; }

 

int main (void)

{

    list<string> Birds;

    list<string>::iterator NewEnd;

 

    Birds.push_back("cockatoo");

    Birds.push_back("galah");

    Birds.push_back("cockatoo");

    Birds.push_back("rosella");

    Birds.push_back("king parrot");

 

    cout << "Original list" << endl;

    for_each(Birds.begin(), Birds.end(), PrintIt);

 

    NewEnd = remove(Birds.begin(), Birds.end(), "cockatoo");

 

    cout << endl << "List according to new past the end iterator" << endl;

    for_each(Birds.begin(), NewEnd, PrintIt);

 

    cout << endl << "Original list now. Care required!" << endl;

    for_each(Birds.begin(), Birds.end(), PrintIt);

}

 

��������Ϊ��

Original list

cockatoo

galah

cockatoo

rosella

king parrot

 

List according to new past the end iterator

galah

rosella

king parrot

 

Original list now. Care required!

galah

rosella

king parrot

rosella

king parrot

ͨ��remove()�㷨����һ��ָ���µ�list�Ľ�β��iterator���ӿ�ʼ������µĽ�β�������½�βԪ�أ��ķ�Χ ������remove��ʣ������Ԫ�ء��������list��Ա����erase������ɾ�����½�β���Ͻ�β�Ĳ��֡�

 

 

--------------------------------------------------------------------------------

 

17.ʹ��STLͨ���㷨stable_partition()��list��Ա����splice()������һ��list

 

ʹ��STLͨ���㷨stable_partition()��list��Ա����splice()������һ��list

�������ǽ����һ����΢�е㸴�ӵ����ӡ�����ʾSTLͨ���㷨stable_partition()�㷨��һ��list��Ա���� splice()�ı仯��ע�⺯�������ʹ�ú�û��ʹ��ѭ���� ͨ���򵥵�������STL�㷨�����ơ�

stable_partition()��һ����Ȥ�ĺ���������������Ԫ�أ�ʹ������ָ��������Ԫ������ ������������Ԫ��ǰ�档��ά��������Ԫ�ص�˳���ϵ��

 

splice ����һ��list�е�Ԫ�ؽ�ϵ�һ��list�С�����Դlist��ɾ��Ԫ�ء�

 

����������У�������������н���һЩ��־���ĸ��ļ������ļ������롯��˳����֡�ͨ��ʹ��stable_partition() ���ǿ��Խ��պ��ļ�����Ϊ�κ�λ�õı�־�����Ҳ������ļ�����˳��Ͱ����Ƿŵ�һ��

 

���ڼ����Ͳ����㷨�������ã����ǵ�����Щ�㷨�������ĸ���־�����ö��ĸ���־δ�����á� �ҷ����������������������������Ķ�̬���ݡ�

 

/*

|| Using the STL stable_partition algorithm

|| Takes any number of flags on the command line and

|| four filenames in order.

*/

#include <string>

#include <list>

#include <algorithm>

 

PrintIt ( string& AString { cout << AString << endl; }

 

class IsAFlag {

public:

    bool operator () (string& PossibleFlag) {

        return PossibleFlag.substr(0,1)=="-";

    }

};

 

class IsAFileName {

public:

    bool operator () (string& StringToCheck) {

        return !IsAFlag()(StringToCheck);

    }

};

 

class IsHelpFlag {

public:

    bool operator () (string& PossibleHelpFlag) {

        return PossibleHelpFlag=="-h";

    }

};


int main (int argc, char *argv[]) {

 

    list<string> CmdLineParameters; // the command line parameters

    list<string>::iterator StartOfFiles; // start of filenames

    list<string> Flags; // list of flags

    list<string> FileNames; // list of filenames

 

    for (int i = 0; i < argc; ++i)

    CmdLineParameters.push_back(argv[i]);

 

        CmdLineParameters.pop_front(); // we don't want the program name

 

    // make sure we have the four mandatory file names

    int NumberOfFiles(0);

    count_if(CmdLineParameters.begin(), CmdLineParameters.end(), IsAFileName(), NumberOfFiles);

 

    cout << "The " << (NumberOfFiles == 4 ? "correct " : "wrong ") << "number (" << NumberOfFiles << ") of file names were specified" << endl;

 

 // move any flags to the beginning

    StartOfFiles = stable_partition(CmdLineParameters.begin(), CmdLineParameters.end(), IsAFlag());

 

    cout << "Command line parameters after stable partition" << endl;

    for_each(CmdLineParameters.begin(), CmdLineParameters.end(), PrintIt);

 

    // Splice any flags from the original CmdLineParameters list into Flags list.

    Flags.splice(Flags.begin(), CmdLineParameters, CmdLineParameters.begin(), StartOfFiles);

 

    if (!Flags.empty()) {

        cout << "Flags specified were:" << endl;

        for_each(Flags.begin(), Flags.end(), PrintIt);

    }

    else {

        cout << "No flags were specified" << endl;

    }

 

    // parameters list now contains only filenames. Splice them into FileNames list.

    FileNames.splice(FileNames.begin(), CmdLineParameters, CmdLineParameters.begin(), CmdLineParameters.end());

 

    if (!FileNames.empty()) {

        cout << "Files specified (in order) were:" << endl;

        for_each(FileNames.begin(), FileNames.end(), PrintIt);

    }

    else {

        cout << "No files were specified" << endl;

    }

 

    // check if the help flag was specified

    if (find_if(Flags.begin(), Flags.end(), IsHelpFlag())!=Flags.end()) {

        cout << "The help flag was specified" << endl;

    }

 

    // open the files and do whatever you do

 

}
