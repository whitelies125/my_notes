# The C++ string Class

C++ 字符串类。

C++ 使用 string 类处理字符串。

string 中的函数，有很多，在这里简单地分个类：

1. 构造
2. 追加
3. 赋值
4. 位置与清除
5. 长度与容量
6. 比较
7. 子串
8. 搜索
9. 运算符

操作 string 对象中的字符串内容时，有时会用到 index。

很多 string 函数接受两个数字参数：index，n。
index：从 index 位置开始（第 1 个字符的 index 为 0）。
n：到包含其及其之后的共 n 个字符。

## Constructing a String

创建 string 对象。

Create an empty string using string's no-arg constructor.

用无参构造函数创建一个空字符串。

~~~C++
string newString;
~~~

Create a string object from a string value or from an array of characters.

由一个字符串常量或字符串数组创建 string 对象。

~~~C++
string message{"aloha world!"}

char charArray[] = {'H','e','l','l','o','\0'};
string message1 {charArray};
~~~

## Appending a String

追加字符串。

You can use several overloaded function to add new contents to a string.

一系列的重载函数可将新内容附加到一个字符串中。

~~~C++
#include <string>
using std::string;

string s1{"Welcome"};
s1.append(" to C++");//appends " to C++" to s1
cout << s1 << endl;//Welcome to c++

string s2{"Welcome"};
s2.append(" to C and C++",3,2);//appends " C" to s2
cout << s2 << endl;//Welcome C

string s3{"Welcome"};
s3.append(" to C and C++",5);//appends " to C" to s3
cout << s3 << endl;//Welcome to C

string s4{"Welcome"};
s4.append(4,'G');//appends "GGGG" to s4
cout << s4 << endl;//WelcomeGGGG
~~~

## Assigning a String

为字符串赋值。

You can use several overloaded functions to assign new contents to a string.

一系列的重载函数可以将一个字符串赋以新内容。

~~~C++
string s1{"Welcome"};
s1.assign("Dallas");//assigns "Dallas" to s1
cout << s1 << endl;//Dallas

string s2{"Welcome"};
s2.assign("Dallas, Texas",1,3);//assigns "all" to s2
cout << s2 << endl;//all

string s3{"Welcome"};
s3.assign("Dallas, Texas",6);//assigns "Dallas" to s3
cout << s3 << endl;//Dallas

string s4{"Welcome"};
s4.assign(4,'G');//assigns "GGGG" to s4
cout << s4 << endl;//GGGG
~~~

## Functions at, cleaa, erase, and empty

- at(index)：返回当前字符串中 index 位置的字符。
- clear()：清空字符串。
- erase(index, n)：删除字符串从 index 开始的含 index 及其后的共 n 个字符。
- empty()：检测字符串是否为空。

~~~c++
string s1{"Welcome"};

cout << s1.at(3) << endl;//c

cout << s1.erase(2,3) << endl;//Weme，删除下标 2 开始及其之后共 3 个字符。
cout << s1 << endl;//Weme
cout << s1.erase(1) << endl;//W，删除下标 1 及其后所有字符

s1.clear();
cout << s1.empty() << endl;//1
~~~

## Comparing Strings

比较字符串。

compare() 函数用于比较两个字符串，与 C 语言中的 strcmp() 函数很像。

~~~C++
string s1{"Welcome"};
string s2{"Welcomg"};

cout << s1.compare(s2) << endl;
//课程老师说返回-2，即 ASCII 码的 e-g，但我自己 code::block 实测返回的是 -1，反正都 <0
cout << s2.compare(s1) << endl;
//课程老师说返回2，即 ASCII 码的 g-e，但我自己 code::block 实测返回的是 1，反正都 >0
cout << s1.compare("Welcome") << endl;//0，一样的
~~~

## Obtaining Substrings

获取子串。

at() 用于获取一个单独的字符；而 substr() 可获取一个子串。

~~~C++
string s1{"Welcome"};

cout << s1.substr(0,1) << endl;//W
cout << s1.substr(3) << endl;//come
cout << s1.substr(3,3) << endl;//com
~~~

## Searching in a String

find() 函数可以在一个字符串中搜索一个子串或一个字符。

~~~C++
string s1{"Welcome to c++"};

cout << s1.find("co") << endl;//3，返回子串出现的第一个位置
cout << s1.find("co", 6) << endl;//从下标 6 开始（包含下标 6）查找子串出现的第一个位置
//课程说：-1
//但我实测返回的是 18446744073709551615
//18446744073709551615 就是 64 bit 的 
//11111111 11111111 11111111 11111111 11111111 11111111 11111111 11111111
//如果改成 (int)s1.find("co", 6)，那么输出值就是 -1。
//而问题在于 find() 返回的是无符号整型
//所以就会以无符号整数来输出，即 2^64-1 = 18446744073709551615 
cout << s1.find('o') << endl;//4，返回字符穿线的第一个位置
cout << s1.find('o', 6) << endl;//9，从下标 6 开始（包含下标 6）查找字符出现的第一个位置
~~~

## Insert and Replacing Strings

插入和替换字符串。

insert()：将某个字符/字符串插入到当前字符串的某个位置。

replace()：将本字符串从某个位置开始的一些字符替换为其它内容。

~~~C++
string s1{"Welcome to C++"};
s1.insert(11, "Java and ");//在下标 11 处前插入
cout << s1 << endl;//Welcome to Java and C++

string s2{"AA"};
s2.insert(1, 4, 'B');//在下标 1 前插入 4 个 'B'
cout << s2 << endl;//ABBBBA

string s3{"Welcome to python and Java"};
s3.replace(11, 6, "C++");//在下标 11 包含 11 及其后共 6 个字符替换为 C++。
cout << s3 << endl;//Welcome to C++ and Java
~~~

## String Operatros

字符串运算符

| Operator             | Description                                      |
| -------------------- | ------------------------------------------------ |
| []                   | 用数组下标运算符访问字符串中的字符               |
| =                    | 将一个字符串的内容复制到另一字符串中             |
| +                    | 链接两个字符串得到一个新串                       |
| +=                   | 将一个字符串追加到另一个字符串末尾               |
| <<                   | 将一个字符串插入一个流                           |
| >>                   | 从一个流中提取一个字符串，分界符为空格或空结束符 |
| ==, !=, <, <=, >, >= | 用于字符串比较                                   |

## 类型转换

~~~C++
std::stoi();//string to int
std::to_string();//else to string
~~~

当然还有其它很多自带的转换函数。
