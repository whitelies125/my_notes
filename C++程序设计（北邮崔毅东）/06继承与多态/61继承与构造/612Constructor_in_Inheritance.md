# Constructor in Inheritance

继承中的构造函数。

## 派生类继承的成员

C++11：派生类不继承的特殊函数：

- 析构函数
- 友元函数

继承基类构造函数：

- using Base_Class_Name::Base_Class_Name; 

  继承所有基类 ctor。
  不能仅继承指定的某个基类 ctor。

~~~C++
struct A{//等同于 class A { public:
    A(int i) {};
    A(double d, int i) {};
};
struct B : A{
    using A::A;//表示继承 A 的所有构造函数。
    int d{0};
};

int main(){
    B b{1};//会调用 A(int i);
    return 0;
}
~~~

相当于：

~~~C++
struct A{//等同于 class A { public:
    A(int i) {};
    A(double d, int i) {};
};
struct B : A{
    B(int i) : A{int i}{}
    B(double d, int i) : A{double d, int i} {};
    int d{0};
};

int main(){
    B b{1};//会调用 A(int i);
    return 0;
}
~~~

## Calling Base Class Constructor

调用基类构造函数。

若派生类成员有需要初始化，则可以在派生类构造函数中调用基类构造函数。

~~~C++
#include <iostream>
struct A{//等同于 class A { public:
    A(int i) {
        std::cout << "A(int i)" << std::endl;
    };
    A(double d, int i) {};
};
struct B : A{
    using A::A;//继承 A 中除了 A(int i) 外的所有构造函数。
                //因为 B 中已有一个自定义的 B(int i) 了。
    B(int i) : A{i}, d{i}{//初始化列表中，是先调用基类的构造函数，然后才是本类的数据成员初始化。
        std::cout << "B(int i)" << std::endl;
    }
    //初始化列表写成下一行的 d{i}, A{i} 的顺序，其执行顺序仍然是 A{i}，然后 d{i}。
    //B(int i) : d{i}, A{i} { std::cout << "B(int i)" << std::endl; }
    int d{0};
};

int main(){
    B b{1};
    return 0;
}
//output:
//A(int i)
//B(int i)
~~~

## Example

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};
class D : public B{
};
//D 没写东西
//编译器自动生成一个默认构造函数。
//D 继承自 B
//D 的默认构造函数会调用 B 的默认构造函数。
//所以下面 D d 会输出 B()
int main(){
    B b;
    D d;
    return 0;
}
//g++ test.cpp -o test.exe
//output:
//B()
//B()
~~~

我自己在这里发现了一个 C++ 版本的问题。

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};
class D : public B{
};
//D 没写东西
//编译器自动生成一个默认构造函数。
//D 继承自 B
//D 的默认构造函数会调用 B 的默认构造函数。
//所以下面 D d 会输出 B()
int main(){
    B b;
    D d;
    D d2{2};
   	D d3{'c'};
    //使用 g++ test.cpp -o test.exe，即 C++14 时，上面两行会报错。
	//而使用 g++ -std=c++17 test.cpp -o test.exe 时，可以正常编译运行。
    return 0;
}
//g++ -std=c++17 test.cpp -o test.exe
//.\test.exe
//output:
//B()
//B()
//B(int i)
//B(char c)
~~~

加上 using B::B

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};
class D : public B{
public:
    using B::B;
    //相当于有
    //D() : B() {}
    //D(int i) : B(i){}
    //D(char c) : B(c){}
    //测试了，把 using B::B 换成这三行注释，输出也一样的。
    
    //但是注意，与下面去掉初始化列表中的基类构造函数有所不同，
	//下面这种情况只会调用基类的无参默认构造函数
    //详见下一节 Default Constructor in Inheritance
    //D(){}
    //D(int i){}
    //D(char c){}
};
int main(){
    B b;
    D d;
	D d2{2};
   	D d3{'c'};
    return 0;
}
//g++ test.cpp -o test.exe
//output:
//B()
//B()
//B(int i)
//B(char c)
~~~

using B::B 换掉：

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};
class D : public B{
public:
    //相当于 using B::B;
    D() : B() {}
    D(int i) : B(i){}
    D(char c) : B(c){}
};
int main(){
    B b;
    D d;
	D d2{2};
   	D d3{'c'};
    return 0;
}
//g++ test.cpp -o test.exe
//output:
//B()
//B()
//B(int i)
//B(char c)
~~~

在换成去掉初始化了列表中的基类构造函数：

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};
class D : public B{
public:
    //从 output 看出，会自动调用基类的默认构造函数。
    D() {}
    D(int i) {}
    D(char c) {}
};
int main(){
    B b;
    D d;
	D d2{2};
   	D d3{'c'};
    return 0;
}
//g++ test.cpp -o test.exe
//output:
//B()
//B()
//B()
//B()
~~~

关于内嵌对象：

派生类先调用基类 ctor，再构造内嵌对象。

~~~C++
#include <iostream>
class B{
public:
    B() { std::cout << "B()" << std::endl; }
    B(int i) { std::cout << "B(int i)" << std::endl; }
    B(char c) { std::cout << "B(char c)" << std::endl; }
};

class E{
public:
    E() { std::cout << "E()" << std::endl; }
};

class D : public B{
private:
    E e1;
    E e2;
public:
    using B::B;
    //先调用基类构造函数 B(int x),然后才是内嵌对象 E()
    //B() 写前面后的都行，反正次序是先调它。
    //最后才是 D(){} 的函数体
    D(double x) : e1{}, e2{}, B(static_cast<int> (x)) {
        std::cout << "D(double x)" << std::endl;
    }
};
int main(){
    D d4{1.23};
    return 0;
}
//output:
//B(int i)
//E()
//E()
//D(double x)
~~~

