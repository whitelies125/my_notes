## typeid

运行时查询类型的信息。

## typeid operator

typeid 运算符。

**typeid** is used to obtain the information about the class of the object.

typeid 用于获取对象所属的类的信息。

- typeid returns a reference to an object of class type_info.

  typeid 返回一个 type_info 对象的引用。

- typeid(Atype).name() 返回实现定义的，含有类型名字的 C 风格字符串（char*）

~~~C++
#include <iostream>
#include <string>
#include <typeinfo>//使用 typeid 需要包含该头文件

class A{};

int main(){
    A a;
    //下面这段代码，到 return 0 前是在运行时才检测。
    auto& t1 = typeid(a);//用 auto 会报错，auto& 才不会
    if(typeid(A) == t1){//如果 a 的类型 是 A 的类型
        std::cout << "a has type " << t1.name() << std::endl;
    }

    return 0;
}
//output
//a has type 1A
//因为我使用的是 g++ 所以输出的是 1A，因为 C++ 没有规定固定返回类型名字，换一个编译器，可能就会返回 class A 了。
~~~

相关概念：

C++ 中有个叫做 RTTI（run-time type information or run-time type identification，运行时类型信息/识别）的东西，自己查吧。
