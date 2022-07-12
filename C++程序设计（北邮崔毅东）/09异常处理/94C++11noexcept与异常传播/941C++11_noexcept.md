# C++11: noexcept

C++11 的 noexcept。

## Why noexcept

C++03 将 throw(ExceptionType)  放到函数后面，说明函数会抛出什么类型的异常，也被称为 " 异常规约 "。

虽然没什么人用就是了。

C++11 改为了：使用 noexcept 指明函数是否抛出异常。

- 若函数不抛异常，在函数后面加上 noexcept 说明，则可做编译优化。
- 即便函数抛异常，不加 noexcept，同时也不再说明所抛异常类型。

## Usage of noexcept specifier

noexcept 声明符的用法。

noexcept 声明符语法：noexcept 或者 noexcept(布尔表达式)

~~~C++
void foo() noexcept {}
void foo() noexcept(true) {}//noexcept(true) 等价于 noexcept
void foo() {}//可能会抛出异常
void foo() noexcept(false) {}//noexcept(false) 等价于什么也不写，可能会抛出异常
~~~

noexcept 不能用于区分重载函数（函数名后的 const 倒是可以区分重载）。

noexcept 函数中抛出异常，等于调用 std::terminate()

~~~C++
void f(){//潜在抛出异常
    //...
    //如果抛出异常，该语句后，则也会直接调用 std::terminate() 程序终止运行
}

void g() noexcept {//noexcpet 修饰
    f();//合法，仍然也可以调用可能会抛出异常的函数。
    throw 42;//合法，该语句后下一条，会直接调用 std::terminate() 程序终止运行
}
~~~

实测一：

~~~C++
#include <iostream>
#include <exception>
class RangeException : public std::exception{
private:
    std::size_t dimension{3};
    int index{0};
public:
    RangeException(std::size_t dimension, const int index)
    : dimension{dimension}, index{index}
    {};
    ~RangeException() = default;
    const char* what() const _GLIBCXX_TXN_SAFE_DYN _GLIBCXX_NOTHROW override {
        return "out of range";
    }
};


void f(){
    throw RangeException(3,4);//抛出异常
}
void g() noexcept {//noexcpet 修饰
    f();
    throw 42;
}

int main(){
    try{
        g();
    }catch(RangeException& e){//使 RangeException 从 exception 类继承
        std::cout << e.what() << std::endl;
    }
    
    return 0;
}
//g++ test.cpp -o test.exe
//.\test.exe

//test.cpp: In function 'void g()':
//test.cpp:24:5: warning: 'throw' will always call 'terminate' [-Wterminate]
//   24 |     throw 42;//錿d::terminate();
//      |     ^~~~~~~~
//terminate called after throwing an instance of 'RangeException'
//  what():  out of range
~~~

实测二：

~~~C++
#include <iostream>
#include <exception>

void g() noexcept {//noexcpet 修饰
    throw 42;
}
int main(){
    try{
        g();
    }catch(int e){//使 RangeException 从 exception 类继承
        std::cout << e << std::endl;
    }
    return 0;
}
//g++ test.cpp -o test.exe
//.\test.exe
//test.cpp: In function 'void g()':
//test.cpp:5:5: warning: 'throw' will always call 'terminate' [-Wterminate]
//    5 |     throw 42;//錿d::terminate();
//      |     ^~~~~~~~
//terminate called after throwing an instance of 'int'
~~~

## Usage of noexcept operator.

noexcept 运算符的用法。

noexcept 既是前面说的标识符，同时也是一个一元运算符。

~~~C++
bool noexcept(expression)
~~~

noexcept 运算符进行**编译时**检查，若表达式声明为不抛出任何异常则返回 true。

~~~C++
void may_throw();
void no_throw() noexcept;
int main() {
    std::cout << std::boolalpha//设置输出 bool 为 ture/false 而不是 1/0
    		<< "Is may_throw() noexcept? "
    		<< noexcept (may_throw()) << '\n'
    		<< "Is no_throw() noexcept? "
    		<< noexcept (no_throw()) << '\n';
    return 0;
}
//output:
//Is may_throw() noexcept? false
//Is no_throw() noexcept? true
~~~

自己实测：

~~~C++
#include <iostream>
#include <exception>

void may_throw();
void no_throw() noexcept{
    throw 42;
}
int main() {
    std::cout << std::boolalpha//设置输出 bool 为 ture/false 而不是 1/0
    		<< "Is may_throw() noexcept? "
    		<< noexcept (may_throw()) << '\n'
    		<< "Is no_throw() noexcept? "
    		<< noexcept (no_throw()) << '\n';
    return 0;
}
//g++ test.cpp -o test.exe
//.\test.exe

//output:
//test.cpp: In function 'void no_throw()':
//test.cpp:6:5: warning: 'throw' will always call 'terminate' [-Wterminate]
//    6 |     throw 42;
//      |     ^~~~~~~~
//Is may_throw() noexcept? false
//Is no_throw() noexcept? true
~~~

## Example

noexcept 不能区分重载：

~~~C++
void foo() {}
void foo() noexcept {}
int main() {
    foo();
    return 0;
}
//g++ test.cpp -o test.exe
//下面提示信息说明 noexcept 不能区分重载函数,顺带 const 可以区分
//test.cpp:5:6: error: redefinition of 'void foo()'
//    5 | void foo() noexcept {}
//      |      ^~~
//test.cpp:4:6: note: 'void foo()' previously defined here
//    4 | void foo() {}
//      |      ^~~
~~~

在 noexcept 函数中抛出异常：

~~~C++
#include <iostream>

void foo() noexcept { throw 1; }
int main() {
    foo();
    return 0;
}
//g++ test.cpp -o test.exe

//编译会警告 warning 但还是能编译出 test.exe
//test.cpp: In function 'void foo()':
//test.cpp:4:23: warning: 'throw' will always call 'terminate' [-Wterminate]
//    4 | void foo() noexcept { throw 1; }
//      |                       ^~~~~~~

//./test.exe

//执行到 foo() noexcept 中 throw 1 后，会直接调用 std::terminate() 终止程序运行。
//terminate called after throwing an instance of 'int'
~~~

noexcept，noexcept(true)，noexcept(false)：

~~~C++
#include <iostream>

void foo() noexcept {}
void tao() noexcept(1) {}
void zen() noexcept(1-1) {}

int main() {
    std::cout << noexcept(foo()) << std::endl;
    std::cout << noexcept(tao()) << std::endl;
    std::cout << noexcept(zen()) << std::endl;
    return 0;
}
//output:
//1
//1
//0
~~~

