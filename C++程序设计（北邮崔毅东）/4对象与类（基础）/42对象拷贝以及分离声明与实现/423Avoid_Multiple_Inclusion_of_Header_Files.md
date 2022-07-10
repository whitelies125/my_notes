# Avoid Multiple Inclusion of Header Files

避免头文件被多次包含

C/C++使用预处理指令（Preprocessing Directives）保证头文件只被包含/编译一次。

~~~C++
#ifndef  MY_HEADER_FILE_H
#define MY_HEADER_FILE_H
//...头文件内容
#endif
~~~

注意每个头文件的 #define 宏名 不能相同。

~~~C++
#pragma once//C++03,C90
//#Pragma 是预处理指令
~~~

放在头文件开头，避免了前一种方式需要给每一个头文件取不同的宏名的麻烦。

~~~C++
_Pragma("once")//C++11,C99;
//_Pragma 实际是一个运算符 operator，因此可以嵌套在宏中
//虽然目前我也不太懂，不过至少知道 _Pragma("once") 单独使用和前面两种方法的效果是一样的。

//也可以加分号,可加可不加。
_Pragma("once");//
~~~

