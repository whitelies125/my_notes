to do



union

枚举 enum

强枚举类型 enum class ClassName: type{...}

~~~C++
enum class Gender:int{//可指定枚举的底层类型，可指定为其它类型 char,unsigned int,...若省略则默认为 int
    male,
    female,//最后这个逗号,可写可不写，C++11 会自动帮我们处理这最后一个逗号。
};
~~~

namespace

using

将亡值，左值，右值，左值引用，右值引用。

gdb

makefile

cmake

gcc g++ 命令

函数指针

智能指针

define

typeof 

~~~C++
typeof int[100] T;
T a;//a:int[100]
~~~

科里化

std::move

循环不变式