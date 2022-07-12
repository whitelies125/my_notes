# Deep Copy

深拷贝。

注意一点，至少我在 C++11 primer（第 5 版）中，是没有找到哪里出现了深拷贝、浅拷贝这两个名词。
而仅仅在第 13 章-13.2 拷贝控制和资源管理中找到了 13.2.1 行为像值的类和 13.2.2 定义行为像指针的类，这两节的内容就是讲对象拷贝时，对其指针类型的数据成员，是仅直接拷贝指针类型数据成员的值，还是对指针类型数据成员所指向的内容进行拷贝。

也就是这一节老师讲的浅拷贝、深拷贝的区别。

So，可能，C++ 里也许根本没有浅拷贝、深拷贝的概念。
Maybe，只是老师为了方便理解，所以将其它语言中的名词借用过来吧。

## Shallow Copy vs. Deep Copy

浅拷贝和深拷贝。

**Shallow copy**: if the field is a pointer to some object, the address of the pointer is copied tather than its contents.

浅拷贝：数据域是一个指针，那么只拷贝指针的地址，而不会拷贝指针指向的内容。

**Deep copy**: Copy the contents that pointed by pointer.

深拷贝：会拷贝指针指向的内容。

即，浅拷贝后两对象的指针成员指向的是同一地址，而深拷贝后两对象的指针成员指向的是不同地址，内容也拷贝了一份。

什么时候会发生浅拷贝：

- Implicit/default copy ctor.

  创建新对象时，调用类的的隐式/默认构造函数。

  这里有点奇怪了，上面这这句中英文都是老师写的，但是英语是 copy ctor，而中文却是构造函数，没有拷贝。没明白这老师到底是什么意思，是调用默认拷贝构造函数，还是默认构造函数，这个默认构造函数是否包括默认拷贝构造函数。这里没讲清。

  总之，自己知道浅拷贝是拷贝的地址，指向同一内容就好。

- default assignment operator for copying =.

  为已有对象赋值的默认赋值运算符。

浅拷贝例子：

~~~C++
#include <iostream>
#include <string>
class Date{
private:
    int year;
    int month;
    int day;
public:
    Date() = default;
    Date(int year, int month, int day)
    : year{year},month{month}, day{day}{}
    
    void print_birthday_string(){
        std::cout << year << "." << month << "." << day << std::endl;
    }
};

class Student{
private:
    std::string name;
    std::string * hobby;
    int number;
    Date* birthday;
public:
    Student(std::string name, int number, std::string * hobby, Date* birthday)
    : name{name}, number{number}, hobby{hobby}, birthday{birthday}{}
    
    //这里并没有写拷贝函数，编译器自动生成一个，从下面结果看出，显然默认的是浅拷贝。
    //Student(const Student& s) = default;

    void print_birthday(){//输出一些信息
        std::cout << "name : " << name << std::endl;
        std::cout << "number : " << number << std::endl;
        std::cout << "hobby content : " << *hobby << std::endl;
        std::cout << "birthday content : ";
        birthday->print_birthday_string();
        std::cout << "name's address : " << &name << std::endl;
        std::cout << "number's address : " << &number << std::endl;
        std::cout << "hobby content : " << hobby << std::endl;
        std::cout << "birthday : " << birthday << std::endl;
    }
};

int main(){
    std::string s = "Bass";
    std::string * hobby = &s;
    Student mio{"mio", 1, hobby, new Date(2022,1,1)};
    mio.print_birthday();
    
    std::cout << std::endl;
    
    Student mio_2 { mio };
    mio_2.print_birthday();
    return 0;
}
//输出
//name : mio
//number : 1
//hobby content : Bass
//birthday content : 2022.1.1
//name's address : 0x61fd70
//number's address : 0x61fd98
//hobby content : 0x61fdb0
//birthday : 0x6e6ea0
//
//name : mio
//number : 1
//hobby content : Bass//内容相同
//birthday content : 2022.1.1//内容相同
//name's address : 0x61fd30
//number's address : 0x61fd58
//hobby content : 0x61fdb0//地址相同
//birthday : 0x6e6ea0//地址相同
~~~

浅拷贝可能会导致一些问题，比如 B 拷贝 A 后，修改 B 的数据成员，但由于浅拷贝，导致 A 的数据成员也同时被修改了。

## Custonmizing Copy Constructor

定制拷贝构造函数。

如何深拷贝？

- 自行编写拷贝构造函数
- 重载赋值运算符

下面是自行编写拷贝构造函数的例子，重载赋值运算符暂且还没学到，所以暂不举例。

~~~C++
#include <iostream>
#include <string>
class Date{
private:
    int year;
    int month;
    int day;
public:
    Date() = default;
    Date(int year, int month, int day)
    : year{year},month{month}, day{day}{}
    
    void print_birthday_string(){
        std::cout << year << "." << month << "." << day << std::endl;
    }
};

class Student{
private:
    std::string name;
    std::string * hobby;
    int number;
    Date* birthday;
public:
    Student(std::string name, int number, std::string * hobby, Date* birthday)
    : name{name}, number{number}, hobby{hobby}, birthday{birthday}{}
    
    //本质上就是对指针类型数据成员，新开内存拷贝其指向的内容
    //然后拷贝的指针类型数据成员并非单纯拷贝值，而是使其指向新开内存中的拷贝内容。
    Student(const Student& s)
    : name{s.name}, number{s.number},
    hobby{new std::string(*(s.hobby))},birthday{new Date(*(s.birthday))}{}

    void print_birthday(){//输出一些信息
        std::cout << "name : " << name << std::endl;
        std::cout << "number : " << number << std::endl;
        std::cout << "hobby content : " << *hobby << std::endl;
        std::cout << "birthday content : ";
        birthday->print_birthday_string();
        std::cout << "name's address : " << &name << std::endl;
        std::cout << "number's address : " << &number << std::endl;
        std::cout << "hobby content : " << hobby << std::endl;
        std::cout << "birthday : " << birthday << std::endl;
    }
};

int main(){
    std::string s = "Bass";
    std::string * hobby = &s;
    Student mio{"mio", 1, hobby, new Date(2022,1,1)};
    mio.print_birthday();
    
    std::cout << std::endl;
    
    Student mio_2 { mio };
    mio_2.print_birthday();
    //可以继续修改 mio_2 的数据成员来观察看看，但我懒就不做这步了
    return 0;
}
//输出：
//name : mio
//number : 1
//hobby content : Bass
//birthday content : 2022.1.1
//name's address : 0x61fd70
//number's address : 0x61fd98
//hobby content : 0x61fdb0
//birthday : 0x696ea0
//
//name : mio
//number : 1
//hobby content : Bass//内容还是相同
//birthday content : 2022.1.1//内容还是相同
//name's address : 0x61fd30
//number's address : 0x61fd58
//hobby content : 0x696ec0//地址不同
//birthday : 0x696f10//地址不同
~~~

