# Destructor

析构函数。

## Destructors are the opposite of constructors.

dtor vs ctor：

- When to invoke? 何时调用？

  Destructor: when the object is destroyed. 对象销毁时自动调用。

  Constructor: when an object is created. 对象创建时自动调用。

- Prototype. 原型

  Destructor: C::~C() 不带参数。

  Constructor: C::C(arguments) 可带参数。

- Default prototype. 默认函数的原型

  Destructor: C::~C()

  Constructor: C::C()

- What if no explicit declaration? 没有显式声明怎么办？

  Destructor: Compiler will create a default one. 编译器会生成默认函数。

  Constructor: Compiler will create a default one. 编译器会生成默认函数。

  就是说，如果我们没有在类中显式地声明构造/析构函数，编译器会自动帮我们生成一个默认的。

  同样的，析构函数也可以像之前构造函数一样使用 default 或 delete：

  ~~~C++
  ~C() = default;//要求编译器自动生成一个析构函数
  ~C() = delete;//要求编译器不生成析构函数。
  ~~~

  但注意，如果对析构函数使用 delete，那么将会导致对象无法释放。所以在这种情况下， 编译器将不允许创建该类型的对象。

- Overloardable? 可否重载？

  Destructor: No,only 1. 不能重载，只有一个析构函数。

  Constructor: Yes. 有多个构造函数。

## Example

注意函数作用域、内嵌作用域中的差异。

~~~C++
//test.h
#include <string>
class Student{
private:
    std::string name;
    int number;
    int age;
    static int num_of_objects;//static member
public:
    Student():Student("mio",1,16){ }//delegating constructor
    Student(std::string name, int number, int age) : name{name},number{number},age{age}{//initializer list
        num_of_objects ++;
    }
    ~Student(){//destructor
        num_of_objects --;
    }
    
    static int get_num_of_objects(){ return num_of_objects; }
};

//test.cpp
#include <iostream>
#include "test.h"

//静态变量在类外定义，同时这里显式初始化为 0 了。静态变量的默认初始化也会为 0，写出来看着明白一些。
int Student::num_of_objects = 0;

int main(){
    Student mio;
    std::cout << Student::get_num_of_objects() << std::endl;//输出: 1
    
    Student* mugi = new Student{"mugi",2,16};//new 出来，方便后面 delete 用来观察变化。
    std::cout << Student::get_num_of_objects() << std::endl;//输出: 2
    
    delete mugi;
    std::cout << Student::get_num_of_objects() << std::endl;//输出: 1
    {//内嵌作用域
        Student yui{"yui",3,16};//在栈区创建 yui
        std::cout << Student::get_num_of_objects() << std::endl;//输出: 2
    }
    //由于 yui 是在栈区创建，当 yui 所在内嵌作用域结束时，yui 被销毁，调用其 destructor，所以 num_of_objects 减 1.
    std::cout << Student::get_num_of_objects() << std::endl;//输出: 1
    
	{
        Student * yui = new Student{"yui",3,16};//使用 new 在堆区创建 yui
        std::cout << Student::get_num_of_objects() << std::endl;//输出: 2
    }
    //因为 yui 是 new 出来的，所以由程序员自己控制销毁，当 yui 所在内嵌作用域结束时，yui 并不会被销毁。
    std::cout << Student::get_num_of_objects() << std::endl;//输出: 2
    return 0;
}
~~~

