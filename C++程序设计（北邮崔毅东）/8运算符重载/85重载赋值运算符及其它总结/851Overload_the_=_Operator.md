# Overload the = Operator

重载赋值运算符。

## The default behavior of the = Operator

赋值运算符的默认行为。

By default , the = operator performs a memberwise copy from one object to the other.

默认情况下，赋值运算符执行对象成员的一对一拷贝。

To change the way th default assignment operator = works, you need to overload the operator.

为了改变其默认的工作方式，可重载赋值运算符。

一般情况下，如果拷贝构造函数需要执行深拷贝，那么赋值运算符需要重载。



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
public:
    std::string name;
    int number;
    std::string * hobby;
    Date* birthday;
public:
    Student() = default;
    Student(std::string name, int number, std::string * hobby, Date* birthday)
    : name{name}, number{number}, hobby{hobby}, birthday{birthday}{}
    
    Student(const Student& s)
    : name{s.name}, number{s.number},
    hobby{new std::string(*(s.hobby))},birthday{new Date(*(s.birthday))}{}

    Student& operator =(const Student& s){
        name = s.name;
        number = s.number;
        *hobby = *(s.hobby);
        *birthday = *(s.birthday);
        return *this;
    }
    void info(){//输出一些信息
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
    mio.info();
    std::cout << std::endl;
    
    Student mio_2 = mio;
    //mio_2 尚未构造，进行拷贝初始化，调用的是拷贝构造函数 Student(const Student& s)
    mio_2.info();
    std::cout << std::endl;

    Student mio_3;//mio_3 已经构造存在
    mio_3 = mio;//调用赋值运算符重载函数 Student& operator =(const Student& s)
    mio_3.info();

    return 0;
}
//输出：
//name : mio
//number : 1
//hobby content : Bass
//birthday content : 2022.1.1
//name's address : 0xd307bffc30
//number's address : 0xd307bffc50
//hobby content : 0xd307bffc70
//birthday : 0x2ad65aafe00
//
//name : mio
//number : 1
//hobby content : Bass
//birthday content : 2022.1.1
//name's address : 0xd307bffbf0
//number's address : 0xd307bffc10
//hobby content : 0x2ad65aaf2e0
//birthday : 0x2ad65aaff20
//
//name : mio
//number : 1
//hobby content : Bass
//birthday content : 2022.1.1
//name's address : 0xd307bffbb0
//number's address : 0xd307bffbd0
//hobby content : 0xd307bffc30
//birthday : 0xd307bffc20
~~~

