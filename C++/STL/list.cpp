

//1 定义一个list

#include <string>
#include <list>

list<string> Milkshakes;

//成员函数
void push_back(const _Ty& _Val);
void push_front(const _Ty& _Val);
void pop_back();
void pop_front();
iterator erase(const_iterator _Where);
bool empty()


//遍历
list<string> list;
list<string>::iterator it;
for (it=list.begin();it!=list.end();it++)
{
    cout << (*it) << endl;
}

//查找函数
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

10.使用STL通用算法search在list中找一个序列

 

一些字符在STL容器中很好处理，让我们看一看一个难处理的字符序列。我们将定义一个list来放字符。

list<char> Characters;

 

现在我们有了一个字符序列，它不用任何帮助就知道然后管理内存。它知道它是从哪里开始、到哪里结束。 它非常有用。我不知道我是否说过以null结尾的字符数组。

 

让我们加入一些我们喜欢的字符到这个list中：

 

Characters.push_back('\0');

Characters.push_back('\0');

Characters.push_back('1');

Characters.push_back('2');

我们将得到多少个空字符呢？

int NumberOfNullCharacters(0);

count(Characters.begin(), Characters.end(), '\0', NumberOfNullCharacters);

cout << "We have " << NumberOfNullCharacters << endl;

让我们找字符'1'

list<char>::iterator Iter;

Iter = find(Characters.begin(), Characters.end(), '1');

cout << "We found " << *Iter << endl;

这个例子演示了STL容器允许你以更标准的方法来处理空字符。现在让我们用STL的search算法来搜索容器中 的两个null。就象你猜的一样，STL通用算法search()用来搜索一个容器，但是是搜索一个元素串，不象find()和find_if() 只搜索单个的元素。

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

The output of the program will be 这是程序的输出：

We found the nulls

 

search算法在一个序列中找另一个序列的第一次出现的位置。在这个例子里我们在ListOfCharacters中 找TargetCharacters这个序列的第一次出现，TargetCharacters是包含两个null字符的序列。 search的参数是两个指着查找目标的iterator和两个指着搜索范围的iterators。 因此我们我们在整个的ListOfCharacters的范围内查找TargetCharacters这个list的整个序列。

 

如果TargetCharacters被发现，search就会返回一个指着ListOfCharacters中序列匹配的第一个 字符的iterator。如果没有找到匹配项，search返回ListOfCharacters.end()的值。

 

 

--------------------------------------------------------------------------------

 

11 使用list的成员函数sort()排序一个list。

 

    要排序一个list，我们要用list的成员函数sort()，而不是通用算法sort()。所有我们用过的算法都是通用算法。然而，在STL中有时容器支持它自己对一个特殊算法的实现，这通常是为了提高性能。

    在这个例子中，list容器有它自己的sort算法，这是因为通用算法仅能为那些提供随机存取里面元素的容器排序，而由于list是作为一个连接的链表实现的，它不支持对它里面的元素随机存取。所以就需要一个特殊的 sort()成员函数来排序list。

    由于各种原因，容器在性能需要较高或有特殊效果需求的场合支持外部函数(extra functions)， 这通过利用构造函数的结构特性可以作到。

 

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

 

输出是：

The unsorted list  John Bill Tony Fidel Nelson The sorted list  Bill Fidel John Nelson Tony

 

 

--------------------------------------------------------------------------------

 

12.用list的成员函数插入元素到list中

 

    list的成员函数push_front()和push_back()分别把元素加入到list的前面和后面。你可以使用insert() 把对象插入到list中的任何地方。

    insert()可以加入一个对象，一个对象的若干份拷贝，或者一个范围以内的对象。这里是一些插入对象到list中的例子：

 

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

注意，insert()函数把一个或若干个元素插入到你指出的iterator的位置。你的元素将出现在 iterator指出的位置以前。

 

 

--------------------------------------------------------------------------------

 

13.List 构造函数

 

我们已经象这样定义了list：

list<int> Fred;

 

你也可以象这样定义一个list，并同时初始化它的元素：

 

// define a list of 10 elements and initialise them all to 0

list<int> Fred(10, 0);

// list now contains 0,0,0,0,0,0,0,0,0,0

或者你可以定义一个list并用另一个STL容器的一个范围来初始化它，这个STL容器不一定是一个list， 仅仅需要是元素类型相同的的容器就可以。

vector<int> Harry;

Harry.push_back(1);

Harry.push_back(2);

#

// define a list and initialise it with the elements in Harry

list<int> Bill(Harry.begin(), Harry.end());

// Bill now contains 1,2

 

 

--------------------------------------------------------------------------------

 

14.使用list成员函数从list中删除元素

 

    list成员函数pop_front()删掉list中的第一个元素，pop_back()删掉最后一个元素。函数erase()删掉由一个iterator指出的元素。还有另一个erase()函数可以删掉一个范围的元素。

 

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

 

输出是：

list contains 0 elements

 

 

--------------------------------------------------------------------------------

 

15.用list成员函数remove()从list中删除元素。

 

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

 

输出是：

  Original list with cockatoos cockatoo galah cockatoo rosella corella Now no cockatoos galah rosella corella

 

 

--------------------------------------------------------------------------------

 

16.使用STL通用算法remove()从list中删除元素

 

通用算法remove()使用和list的成员函数不同的方式工作。一般情况下不改变容器的大小。

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

 

输出结果将为：

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

通用remove()算法返回一个指向新的list的结尾的iterator。从开始到这个新的结尾（不含新结尾元素）的范围 包含了remove后剩下所有元素。你可以用list成员函数erase函数来删除从新结尾到老结尾的部分。

 

 

--------------------------------------------------------------------------------

 

17.使用STL通用算法stable_partition()和list成员函数splice()来划分一个list

 

使用STL通用算法stable_partition()和list成员函数splice()来划分一个list

　　我们将完成一个稍微有点复杂的例子。它演示STL通用算法stable_partition()算法和一个list成员函数 splice()的变化。注意函数对象的使用和没有使用循环。 通过简单的语句调用STL算法来控制。

stable_partition()是一个有趣的函数。它重新排列元素，使得满足指定条件的元素排在 不满足条件的元素前面。它维持着两组元素的顺序关系。

 

splice 把另一个list中的元素结合到一个list中。它从源list中删除元素。

 

在这个例子中，我们想从命令行接收一些标志和四个文件名。文件名必须’按顺序出现。通过使用stable_partition() 我们可以接收和文件名混为任何位置的标志，并且不打乱文件名的顺序就把它们放到一起。

 

由于记数和查找算法都很易用，我们调用这些算法来决定哪个标志被设置而哪个标志未被设置。 我发现容器用来管理少量的象这样的动态数据。

 

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
