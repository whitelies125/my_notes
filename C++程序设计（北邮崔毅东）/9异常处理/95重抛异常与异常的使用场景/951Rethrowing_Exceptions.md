# Rethrowing Exceptions

重抛异常。

## An exception handler can rethrow the exception

异常处理程序可以重新抛出异常

####  When to rethrow an exception?

什么时候重抛异常？

1. if the handler cannot process the exception.

   当它无法处理该异常。

2. the handler simply wants to let its caller be notified.

   或想通知它的调用者发生了一个异常。

#### Rethrow Same Exception

重抛出相同的异常。

~~~C++
try {
	// statements;
}
catch (TheException &ex) {
	// Do something;
	throw;//把当前的异常重新抛出去
}
~~~

#### Rethrow Another Exception

An exception handler can throw another exception other than the captured exception

异常处理程序也可以重新抛出另一个不同于已经捕获异常。

~~~C++
class MyException: public logic_error { };
try {
	throw logic_error();//抛出派生异常对象
} catch (logic_error& e) {//catch参数为基类异常，但可以捕获所有派生类异常对象
	//MyException* p = dynamic_cast<MyException*>(&e);
	//dynamic_cast 转指针失败不会抛异常，而是 p = nullptr
	MyException& p = dynamic_cast< MyException&>(e);
	//dynamic_cast 引用转换失败会才会抛异常 std::bad_cast
    //这里抛出了一个不同于已捕获异常的异常。
	cout << p.what() << endl; // 上面抛异常，本语句被跳过
}
~~~

## Example

~~~C++
#include <iostream>

void f(){
    throw 1;
}

void g(){
    try{
        f();
    }catch(int e){
        std::cout << e << std::endl;
        throw;
    }
}

int main(){
    try{
        g();
    }catch(int e){
        std::cout << e << std::endl;
    }
    return 0;
}
//output:
//1
//1
~~~

