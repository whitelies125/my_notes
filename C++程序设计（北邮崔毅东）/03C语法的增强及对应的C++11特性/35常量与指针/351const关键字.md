# Constant

## const 关键字

const 用于定义常量。

格式：

~~~C++
cosnt datetype CONSTANTNAME = VALUE;
~~~

必须在声明时初始化。

例子

~~~C++
const double PI = 3.14159;
const int SIZE = 3;
int const X = 5;//实际上，写成 cosnt datetype 或 datetype const 都行
const char C = 'k';
const char* STR = "Hello";

PI = 3.14;//error,不能对常量进行修改
~~~

常量是程序中一块数据，这个数据一旦声明后就不能被修改了。
如果这块数据有一个名字，这个名字叫做**命名常量（Named Constant）**或**符号常量**。
如果这块数据（这个常量）从字面上看就能知道它的值，那它叫做**字面常量（Literal Constant）**。

在上述例子中，
等号左边的如 PI、SIZE、X、C、STR 就为命名常量或符号常量；
等号右边的部分，如 3.14159、3、5、'k'、"hello" 就为字面常量。

推荐的命名规范：符号常量必须全部大写，并用下划线分隔单词。