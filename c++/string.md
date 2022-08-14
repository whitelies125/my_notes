# string

[TOC]

## Constants

### npos

special value. The exact meaning depends on the context

在 stringfwd.h 中，

~~~C++
/// A string of @c char
typedef basic_string<char> string;   
~~~

查看 basic_string.h：

~~~C++
template<typename _CharT, typename _Traits, typename _Alloc>
class basic_string
{
	//...
	///  Value returned by various member functions when they fail.
	static const size_type npos = static_cast<size_type>(-1);
    //...
}
~~~

npos 其实就是将 -1 转为 size_type 类型。

### Member functions

### Search

#### find

find characters in the string.

Finds the first substring equal to the given character sequence. Search begins at `pos`, i.e. the found substring must not begin in a position preceding `pos`.

~~~C++
#include <iostream>
#include <string>

int main(){
    std::string str {"abc1_abc2_abc3"};
    std::cout << str.find('a') << std::endl;	// 0
    std::cout << str.find('a',1) << std::endl;	// 5;从下标 1(含 1) 开始查找
    std::cout << str.find("bc") << std::endl;	// 1
    std::cout << str.find("bc",2) << std::endl;	// 6;从下标 2(含 2) 开始查找
    //class basic_string{
    //static const size_type npos = static_cast<size_type>(-1);
    if(str.find('d') == std::string::npos)
        std::cout << "std::string::npos" << std::endl;
    if(str.find('d') == str.npos)
        std::cout << "str.npos" << std::endl;
    return 0;
}
//output:
//0
//5
//1
//6
//std::string::npos
// str.npos
~~~

#### rfind

find the last occurrence of a substring

#### find_first_of

find first occurrence of characters.

Finds the first character equal to one of the characters in the given character sequence. The search considers only the interval [`pos`, size()). If the character is not present in the interval, npos will be returned.

~~~C++
#include <iostream>
#include <string>

int main(){
    std::string str {"abc1_abc2_abc3"};
    std::cout << str.find_first_of('a') << std::endl;//0
    std::cout << str.find_first_of('a',1) << std::endl;//5;从下标 1(含 1) 开始查找
    std::cout << str.find_first_of("cb") << std::endl;//1
    std::cout << str.find_first_of("cb",3) << std::endl;//6;从下标 3(含 3) 开始查找
    //class basic_string{
    //static const size_type npos = static_cast<size_type>(-1);
    if(str.find_first_of('d') == std::string::npos)
        std::cout << "std::string::npos" << std::endl;
    if(str.find_first_of("def") == str.npos)
        std::cout << "str.npos" << std::endl;
    return 0;
}
//output:
//0
//5
//1
//6
//std::string::npos
//str.npos
~~~

#### find_first_not_of

find first absence of characters

#### find_last_of

find last occurrence of characters

#### find_last_not_of

find last absence of characters
