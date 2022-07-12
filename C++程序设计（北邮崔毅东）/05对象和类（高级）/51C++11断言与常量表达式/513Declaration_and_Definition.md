# Declaration and Definition

声明与定义。

## What is "Declare/Declaration".

什么是声明。

A declaration introduces an identifier and describes its type, be it a type, object, or function. A declaration is what the compiler needs to accept references to that identifier.

一个声明引入一个标识符并描述它的类型，无论是类型，对象还是函数。一个声明是编译器所需要的，以便编译器在别处使用该标识符。

~~~C++
extern int bar;//使用 extern 说明 bar 在别的地方，比如别的.cpp 文件中，声明了，但不在我这个当前的文件中。
~~~

我翻了翻《C++11 Primer》

~~~C++
extern int j;//只声明 j，
int j;//声明并定义了 j //默认初始化了
extern int j = 1;//声明并定义了 j，此处 extern 无效。
~~~

一个标识符能且只能被定义一次，但可以被多次声明。

注意的是，由于函数的声明与定义本身就又区别，可以区分开。

~~~C++
//函数的声明，没有函数体{}
int max(int, int);
int max(int a, int b);//声明里变量名也可省略，不过往往写着，方便人看懂
//函数的定义，有函数体
int max(int a, int b){
    return a > b ? a : b;
}
~~~

所以，对于函数来说，其声明可以省略 extern，换句话说，函数的声明，不写 extern 也是隐含了 extern 的

~~~C++
int max(int, int);
int max(int a, int b);
//等同于
extern int max(int, int);
extern int max(int a, int b);
~~~

顺带一提，类 class 的声明但不定义是：

~~~C++
class ClassName;//同样没有花括号
~~~

下面是自己的一些实际尝试：

~~~C++
//test.cpp:
#include <iostream>
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
	std::cout << c << std::endl;
    return 0;
}
//都不用编译，编辑器就自动提示 a,b,c 未定义了。
~~~

加上 extern：

~~~C++
//test.cpp:
#include <iostream>
extern int a;
extern int b;
extern int c;
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
//编辑器的错误提示消失，显示没有错误。
//g++ test.cpp -o test.exe
//编译报错：
//undefined reference to `a'
//undefined reference to `b'
//undefined reference to `c'
~~~

在另一个 test_1.cpp 中定义：

~~~C++
//test_1.cpp
int a = 0;
int b = 1;
int c = 2;
//test.cpp
#include <iostream>
extern int a;
extern int b;
extern int c;
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
//g++ test.cpp test_1.cpp -o test.exe
//顺利通过，没有报错。
//./test
//输出：
//0
//1
//2
~~~

我又换了一种方式用 #Include 尝试：

~~~C++
//test_1.cpp
int a = 0;
int b = 1;
int c = 2;
//test.cpp
#include <iostream>
#include "test_1.cpp"
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
//g++ test.cpp -o test.exe
//顺利通过，没有报错。
//./test
//输出：
//0
//1
//2
~~~

再次尝试，看看 #include 和 extern 的区别：

~~~C++
//test_1.cpp
int a = 0;
int b = 1;
int c = 2;
//test.cpp
#include <iostream>
extern int a;
extern int c;
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << c << std::endl;
    return 0;
}
//此时，编辑器自动提示，b 未定义。
//可见，使用 #include 会将其 include 的文件全部复制粘贴过来，而 extern 则可以指定我只需要那些东西。
~~~

再尝试一下 extern 修饰函数：

~~~C++
//test_1.cpp
int a = 0;
int b = 1;
int c = 2;
int max(int a, int b){
    return a > b ? a : b;
}
int min(int a, int b){
    return a < b ? a : b;
}
//test.cpp
#include <iostream>
extern int a;
extern int b;
extern int max(int a, int b);//声明，我也试了此处 extern 即便去掉也没问题。
int main() {
    std::cout << a << std::endl;
    std::cout << b << std::endl;
    std::cout << max(a,b) << std::endl;
    //std::cout << min(a,b) << std::endl;
    //使用 min(a,b) 会报错，因为没有声明
    //而若换成 #include "test_1.cpp" 则不会报错。
    return 0;
}
//可见，使用 extern 则可以指定的需要的东西，而不像 include 全部拿过来。
//输出：
//0
//1
//1
~~~

## What is "define/definition".

什么是定义。

A definition actually instantiates/implements this identifier. It's what the linker needs in order to link references to those entities.

一个定义实例化/实现这个标识符。定义是链接器所需要的，以便链接器将对标识符的引用链接到标识符所表示的实体。

## Difference between a declaration and a definition

定义与声明的区别。

- A definition can be used in the place of a declaration.

  定义有时可取代声明，反之不行。

- An identifier can be declared more than once, but can be defined only once.

  一个标识符可被声明多次，但只能被定义一次。

- 定义通常伴随着编译器未标识符分配内存。
