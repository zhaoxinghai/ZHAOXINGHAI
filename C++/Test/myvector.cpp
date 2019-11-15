
#include <vector>
using namespace std;

vector<double> vec;
cout << "Size of vec is " << vec.size();
// Output
// Size of vec is 0


vector<double> vec(5);
cout << "Size of vec is " << vec.size();
// Output
// Size of vec is 5

vector<double> vec(5, 8.8);
cout << "Size of vec is " << vec.size() << endl;
cout << "vec: ";
for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";
// Output
// Size of vec is 5
// vec: 8.8 8.8 8.8 8.8 8.8


double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec(array, array + 5);

cout << "vec: ";
for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";

// Output
// vec: 3.45 67 10 0.67 8.99


vector<double> veccopy(vec);

cout << "veccopy: ";
for(int i = 0; i < veccopy.size(); i++)
    cout << veccopy[i] << " ";

// Output
// veccopy: 3.45 67 10 0.67 8.99


vector<double> vec;
double array[5] = {3.45, 67, 10, 0.67, 8.99};
vec.assign(array, array + 5);

cout << "vec: ";
for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";

// Output
// vec: 3.45 67 10 0.67 8.99

vector<double> vec;
vec.assign(5, 8.8);

cout << "vec: ";
for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";

// Output
// vec: 8.8 8.8 8.8 8.8 8.8


vector<double> vec;
vec.assign(5, 8.8);

cout << "vec: ";
for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";

// Output
// vec: 8.8 8.8 8.8 8.8 8.8
vector<double> vec;
vec.assign(5, 8.8);

cout << "vec: ";
for(int i = 0; i < 5; i++)
    cout << vec.at(i) << " ";

// Output
// vec: 8.8 8.8 8.8 8.8 8.8

vector<double> vec;
vec.assign(5, 8.8);

cout << "vec: ";
vector<double>::iterator it;
for(it = vec.begin(); it != vec.end(); it++)
    cout << *it << " ";
// Output
// vec: 8.8 8.8 8.8 8.8 8.8

double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec;
vec.assign(array, array + 5);
vector<double>::iterator it;

it = &vec.front();
cout << *it << " ";
it = &vec.back();
cout << *it;
// Output
// 3.45 8.99

double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec;
vec.assign(array, array + 5);

vector<double>::reverse_iterator rit;
cout << "reverse of vec: ";
for(rit = vec.rbegin(); rit != vec.rend(); rit++)
    cout << *rit << " ";

// Output
// reverse of vec: 8.99 0.67 10 67 3.45


void print(vector<double> vec, char * name)
{
    vector<double>::iterator it;

    cout << name << ": ";
    for(it = vec.begin(); it != vec.end(); it++)
      cout << *it << " ";
}

vector<double> vec;

vec.push_back(2.0);
vec.push_back(4.0);
vec.push_back(6.0);

print(vec, "vec");

// Output
// vec: 2.0 4.0 6.0

vector<double> vec;
vector<double>::iterator it;

vec.push_back(2.0);
vec.push_back(4.0);
vec.push_back(6.0);

// insert value 67 at the position specified by iterator it
it = vec.end();
vec.insert(it, 67);
print(vec, "vec");
// Output
// vec: 2 4 6 67

// insert value 90 three times at the position specified by iterator it
it = vec.begin();
vec.insert(it, 3, 90);
print(vec, "vec");
// Output
// vec: 2 4 6 67
// vec: 90 90 90 2 4 6 67


// insert values from an array at the positon specified by iterator it
double array[5] = {1, 2, 3, 4, 5};
it = vec.begin() + 3;
vec.insert(it, array, array + 5);
print(vec, "vec");

// Output
// vec: 2 4 6 67
// vec: 90 90 90 2 4 6 67
// vec: 90 90 90 1 2 3 4 5 2 4 6 67Removing elements from a vector



double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec(array, array + 5);

vec.pop_back();

print(vec, "vec");

// Output
// vec: 3.45 67 10 0.67

double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec(array, array + 5);
vector<double>::iterator it;

// erase second element of vec
it = vec.begin() + 1;
vec.erase(it);

print(vec, "vec");

// Output
// vec: 3.45 10 0.67 8.99T

double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec(array, array + 5);

vec.clear();

cout << "size of vec is " << vec.size();

// Output
// size of vec is 0

vector<double> vec;

if(vec.empty())
    cout << "vec is empty";
else
    cout << "vec is not empty";

// Output
// vec is emptySizing and resizing a vector

vector<double> vec;
vec.push_back(100);

cout << "vec size is " << vec.size() << endl;
cout << "vec capacity is " << vec.capacity() << endl;
cout << "vec maximum size is " << vec.max_size();

// Output
// vec size is 1
// vec capacity is 1
// vec maximum size is 536870911

vector<double> vec;
vec.push_back(100);

cout << "vec capacity is " << vec.capacity() << endl;
vec.reserve(50);
cout << "vec capacity is " << vec.capacity();

// Output
// vec capacity is 1
// vec capacity is: 50

double array[5] = {3.45, 67, 10, 0.67, 8.99};
vector<double> vec(array, array + 5);

cout << "vec size is " << vec.size() << endl;
print(vec, "vec");

// case when new size <= size of vector
vec.resize(3);
cout << "vec size is " << vec.size() << endl;
print(vec, "vec");

// case when new size > size of vector
vec.resize(10);
cout << "vec size is " << vec.size() << endl;
print(vec, "vec");

// Output
// vec size is 5
// vec: 3.45 67 10 0.67 8.99
// vec size is 3
// vec: 3.45 67 10
// vec size is 10
// vec: 3.45 67 10 0 0 0 0 0 0 0An important thing to notice is that, unlike the reserve() function, which only changes the size of the vector, the resize() function can also change the contents of the vector, by adding new elements or removing existing ones.


//Two dimensional vectors
vector< vector<double> > matrix;

double array1[5] = {1, 3, 5, 7, 9};
vector<double> vec1(array1, array1 + 5);
double array2[5] = {2, 4, 6, 8, 10};
vector<double> vec2(array2, array2 + 5);

matrix.push_back(vec1);
matrix.push_back(vec2);

for(int i = 0; i < matrix.size(); i++)
{
    for(int j = 0; j < matrix[i].size(); j++)
    {
        cout << matrix[i][j] << " ";
    }
    cout << endl;
}

cout << endl;

vector< vector<double> >::iterator it2d;
vector<double>::iterator it1d;
for(it2d = matrix.begin(); it2d != matrix.end(); it2d++)
{
    for(it1d = (*it2d).begin(); it1d != (*it2d).end(); it1d++)
    {
        cout << *it1d << " ";
    }
    cout << endl;
}

// Output
// 1 3 5 7 9
// 2 4 6 8 10
// 
// 1 3 5 7 9
// 2 4 6 8 10Vectors of pointers

vector<double*> vec;

double * d1 = new double(10);
double * d2 = new double(20);
double * d3 = new double(30);

vec.push_back(d1);
vec.push_back(d2);
vec.push_back(d3);

for(int i = 0; i < vec.size(); i++)
    cout << vec[i] << " ";

for(i = 0; i < vec.size(); i++)
    cout << *vec[i] << " ";

// Output
// 00481C30 00481BF0 00481A10 10 20 30


class Point
{
    int x;
    int y;

public:

    Point()
        : x(0), y(0)
    {
    }

    Point(int px, int py)
        : x(px), y(py)
    {
    }

    Point(const Point& pt)
        : x(pt.x), y(pt.y)
    {
        cout << "Inside the copy constructor!" << endl;
    }

    void print()
    {
        cout << "Point: " << x << ", " << y << endl;
    }
};

vector<Point> vecp;

Point p1(1, 5);
Point p2(3, 5);
Point p3;

vecp.push_back(p1);
vecp.push_back(p2);
vecp.push_back(p3);

for(int index = 0; index < vecp.size(); index++)
    vecp[index].print();

// Output
// Inside the copy constructor!
// Inside the copy constructor!
// Inside the copy constructor!
// Inside the copy constructor!
// Inside the copy constructor!
// Inside the copy constructor!
// Point: 1, 5
// Point: 3, 5
// Point: 0, 0As we can see, the copy constructor was called six times. So, to make sure all copying is correctly performed, we must add the copy constructor to a class or a structure if we intend to use it in a vector.

//Avoiding memory leaks when using a vector

class Person
{
    char * name;
    int age;

public:

    Person(char * pname = "Anonymous", int page = 0)
        :age(page)
    {
        name = new char[strlen(pname) + 1];
        strcpy(name, pname);
    }

    Person(const Person& rhs)
    :age(rhs.age)
    {
        name = new char[strlen(rhs.name) + 1];
        strcpy(name, rhs.name);
    }

    void Print()
    {
        cout << name << " " << age << endl;
    }
};

vector<Person> vecp;

Person p1("Bill Gates", 50);
Person p2("John Malcom", 67);
Person p3("Scott Meyers", 34);
Person p4("Mark Gosling", 40);
Person p5;

vecp.push_back(p1);
vecp.push_back(p2);
vecp.push_back(p3);
vecp.push_back(p4);
vecp.push_back(p5);

for(int i = 0; i < vecp.size(); i++)
    vecp[i].Print();

cout << endl;

vecp.resize(2);

for(i = 0; i < vecp.size(); i++)
    vecp[i].Print();

// Output
// Bill Gates 50
// John Malcom 67
// Scott Meyers 34
// Mark Gosling 40
// Anonymous 0
// 
// Bill Gates 50
// John Malcom 67Now, at some point, we may need to resize the vector to a smaller capacity than its current one, say we resize the vector to two elements. The last three elements will be removed from the vector, in order to bring it to the desired size. The elements will be destroyed, but the memory occupied by each name char pointer will not be released (since the standard destructor will be called for each element that is being removed). Thus, memory leaks appear. This is a simple example. In reality, more complicated problems can appear, resulting in huge memory leaks, memory corruption, and worse scenarios.

The solution to avoid memory leaks is very simple. We just need to add a destructor to the Person class to take care of releasing the memory occupied by the char pointer. Thus, each time an element is destroyed, its destructor will be called, which will correctly release the allocated memory.


class Person
{
    ....................................

    ~Person()
    {
        delete [] name;
    }
}

