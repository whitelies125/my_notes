# string

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

find characters in the string

#### rfind

find the last occurrence of a substring

#### find_first_of

find first occurrence of characters

#### find_first_not_of

find first absence of characters

#### find_last_of

find last occurrence of characters

#### find_last_not_of

find last absence of characters



#### string.find(char)

~~~C++
string.find(char)//返回字符串中第一个 char 下标，若无返回 string::nops
~~~
