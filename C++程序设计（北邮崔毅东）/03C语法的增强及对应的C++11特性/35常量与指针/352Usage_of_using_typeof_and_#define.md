# 352 Usage of using, typedef and #define

## 一. C 语言中的 #define 和 typedef

### 1.1 #define

作用：

用来定义宏（macro）。编译器不做类型检查

格式：

~~~c
#define MACRONAME Something
~~~

例子：

~~~c
#define TRUE 1
//结尾无分号，宏名为 TRUE
~~~

### 1.2 typedef

作用：

创建能在任何位置替代类型名的别名。

格式：

~~~c
typedef Sometype NewTypeName;
~~~

例子：

~~~c
typedef _Bool bool;
~~~

## 二. C++ 中的 using

### 2.1 using 用来替代 typedef

作用：

创建类型的别名。

用法：

~~~C++
using identifier = type-id
~~~

例子：

~~~C++
using UInt = unsigned int;
//类型别名，等同于 typedef unsigned int UInt;

using FuncTyep = void(*)(int,int);
//类型别名，等同于 typedef void(*FuncType)(int,int);
//名称 FuncType 现在指代指向函数的指针
~~~

这里函数指针我还不太懂。

### 2.2 定义模板的别名，只能使用 using

如：

~~~C++
using std::cin;//正确
//而不能写成
using mycin = std::cin;//错误。对类型的别名才能用这样的格式写
~~~

