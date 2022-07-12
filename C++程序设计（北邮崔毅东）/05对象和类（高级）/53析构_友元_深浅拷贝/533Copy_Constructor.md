# Copy Constructor

拷贝构造函数 copy ctor。

## Copy Constructions

#### 拷贝构造。

拷贝构造：用一个对象初始化另一个同类对象。

#### 声明拷贝构造函数：

~~~C++
Circle (Circle&);
Circle (const Circle&);
~~~

来自 C++11 primer（第 5 版）：如果一个函数的第一个参数是自生类型的引用，且任何欸外参数都有默认值，则此构造函数是拷贝构造函数。

~~~C++
Circle (Circle&,int i = 1)//所以，这也是拷贝构造函数，满足第一个参数是自生类型的引用。
~~~

这是很显然的，如果形参不为引用类型，则会陷入，实参传递给非引用类型形参时，需要调用拷贝构造函数，而拷贝构造函数时，需要实参传递给非引用类型形参，的困境。

同样的，我们也可以对拷贝构造函数使用 default 或 delete：

~~~C++
Circle (const Circle&) = default;//显式要求编译器生成一个默认拷贝构造函数。
Circle (const Circle&) = delete;//显式要求编译器不生成默认拷贝构造函数，也就阻止了对象拷贝。
~~~

#### Invoke the copy constructor. 调用拷贝 ctor：

~~~C++
Circle c1{ 5.0 };//直接初始化，列表初始化，不会调用拷贝构造函数

Circle c2(c1);//C++03 //调用拷贝构造函数
Circle c3 = c1;//C++03 //虽然是等号，但在这种对象拷贝初始化时，其实是调用拷贝构造函数
Circle c4 = {c1};//C++11 //调用拷贝构造函数
Circle c5{c1};//C++11 //调用拷贝构造函数
~~~

注意，仅当对象拷贝初始化时，等号意味着调用拷贝构造函数，

~~~C++
className obj_1 = obj_2//调用拷贝构造函数 
~~~

而在下述的情形中：

~~~C++
obj_1 = obj_2;//为对象赋值，不调用拷贝构造函数
~~~

等号意味着给对象赋值，将 obj_2 的值赋值给 obj_1，而并不会调用拷贝构造函数。

## Implicitly-declared Copy Constructor

隐式声明的拷贝构造函数。

In general, if the programmer does not provide a copy ctor, the compiler will generate one.

一般情况下，如果程序员不编写拷贝构造函数，那么编译器会自动生成一个。

The generated copy ctor is called " implicitly-declared/defined copy ctor ".

自动生成的拷贝构造函数称为 " 隐式声明的拷贝构造函数 "。

In general, the implicitly-declared/defined copy ctor simply copies each data field in one object to its conterpart in the other object.

一般情况下，隐式声明的拷贝构造函数简单地将作为参数的对象中的每个数据域赋值到新对象中。

来自 C++11 primer（第 5 版）第 13 章-13.2 拷贝控制和资源管理 13.1.1 拷贝构造函数：

拷贝初始化除了上述的使用等号 = 初始化时会发生，还会发生在：

- 将一个对象作为实参传递给一个非引用类型的形参
- 从一个返回类型为非引用的函数返回一个对象
- 用花括号列表初始化一个数组中的元素或一个聚合类中的成员。

某些类类型还会对它们所分配的对象使用拷贝初始化。

补充，聚合类（aggregate class），指：

- 所有成员都是 public
- 没有定义构造函数
- 没有类内初始值
- 没有基类，也没有虚函数

## Example

~~~C++
#include <iostream>
#include <string>
class Student{
private:
    std::string name;
    int number;
    static int num_of_objects;
public:
    Student(){//default constructor
        std::cout << "Student()" << std::endl;
    }
    Student(std::string name, int number)//
    : name(name), number(number){
        std::cout << "Student(string,int)" << std::endl;
        num_of_objects ++;
    }
    Student(const Student& s){//copy constructor
        std::cout << "Student(const Student&)" << std::endl;
        this->name = s.name;//可以正常访问其 private 数据成员。
        this->number = s.number;
        num_of_objects ++;
    }
    ~Student(){ num_of_objects --;}
};
int Student::num_of_objects = 0;
int main(){
    Student mio{"mio",1};//输出：Student(string,int)
	//说明这样的方式调用的是Student(string,int)
    //std::cout << mio.name << std::endl;//报错，数据成员为 private，在这里不能访问
    Student mio_2 = mio;//输出：Student(const Student&)
	//说明拷贝初始化的方式调用的是拷贝构造函数 Student(const Student&)
    return 0;
}
~~~

说明创建 mio_2 时调用的构造函数是拷贝构造函数。

顺带，我发现在 main() 中无法访问 mio.name，因为是 private 但传给类中的函数，反而就能访问了。

还可以将拷贝构造函数使用委托构造来完成：

~~~C++
#include <iostream>
#include <string>
class Student{
private:
    std::string name;
    int number;
    static int num_of_objects;
public:
    Student(){//default constructor
        std::cout << "Student()" << std::endl;
    }
    Student(std::string name, int number)//
    : name(name), number(number){
        std::cout << "Student(string,int)" << std::endl;
        num_of_objects ++;
    }
    Student(const Student& s)//copy constructor
    : Student(s.name,s.number){//delegating constructor
        std::cout << "Student(const Student&)" << std::endl;
    }
    ~Student(){ num_of_objects --;}
};
int Student::num_of_objects = 0;
int main(){
    Student mio{"mio",1};//输出：Student(string,int)
    Student mio_2 = mio;
    //输出：
    //Student(string,int)
	//Student(const Student&)
    return 0;
}
~~~

