# Class Template

类模板。

类模板是将类中某些类型变为泛型，从而定义一个模板

## Generic types of class members

类成员的泛型。

- Data field member: can be of a generic data **type**.

  数据域成员：可以成为泛型数据。

- Function  member: return **type**, parameter **type**, local var **type**.

  函数成员：返回值类型、参数类型、局部变量类型可以成为泛型。

## syntax of class template

类模板的语法。

以栈 stack 为例。

~~~C++
template<typename T>
class Stack{
private:
    T elements[100];
    int size;
public:
    Stack();
    bool empty();
    T peek();
    T push(T value);
    T pop();
    int getSize();
};

template<typename T>//模板类的成员函数，仍然要写成函数模板
bool Stack<T>::empty(){//需要加上 <T>
	return (size == 0);
}
template<typename T>
T Stack<T>::peek(){
	return elements[size - 1];
}
~~~

## Example

test.cpp

~~~C++
#include <iostream>
#include "Stack.h"

int main(){
    Stack<char> c;
    std::string s {"hello,world!"};
    for(char& it : s) c.push(it);
    while(!c.empty()) std::cout << c.pop();
    return 0;
}
//output:
//!dlrow,olleh
~~~

Stack.h

~~~C++
template<typename T>
class Stack{
private:
    T elements[100];
    int size;
public:
    Stack();
    bool empty();
    T peek();
    T push(T value);
    T pop();
    int getSize();
};
//都需要写为
//template <typename T>
//Stack<T>::function_name(){
//	...
//}
//的形式
template <typename T>
Stack<T>::Stack(){
    size = 0;
    for(auto& it : elements) it = 0;
}
template<typename T>
bool Stack<T>::empty(){
	return (size == 0);
}
template<typename T>
T Stack<T>::peek(){
	return elements[size - 1];
}
template<typename T>
T Stack<T>::push(T value){
    elements[size] = value;
    ++size;
    return value;
}
template<typename T>
T Stack<T>::pop(){
    --size;
    return elements[size];
}
~~~

