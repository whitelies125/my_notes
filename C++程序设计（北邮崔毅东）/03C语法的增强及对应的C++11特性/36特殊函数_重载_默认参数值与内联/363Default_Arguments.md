# 363 Default Arguments

C++ allows you to declare functions with default argument values.

C++ 允许声明带有默认参数值的函数。

The default values are passed to the parameters when a function is invoked without the argument.

无参调用函数时，默认参数值会被传递给形式参数。

在定义时需要注意，参数列表中的默认值参数应该**后置**。

~~~C++
void t1( int x, int y = 1, int z = 2);//OK

void t1( int x, int y = 1, int z);//no
~~~

在调用时需要注意，参数列表中的实参应该**前置**。

~~~C++
t1(0);//OK,y,z 为默认值
t1(1,2);//OK,z 为默认值

t1(1, ,20)//no
~~~

(C++03/C++11) A default argument shall not be redefined by a later declaration (not even to the same value).

函数重定义/声明时，不允许重定义默认参数，即便是相同的默认参数。

~~~C++
#include <iostream>

int func_1(int x = 1);

int main(){
    std::cout << func_1(1);
    return 0;
}

int func_1(int x = 2.2){//不行，不能重定义默认参数，即便 x = 1 也不行
    return x;
}

~~~

一个小问题：

~~~C++
int add(int x, int y = 10){
    return x+y;
}

int add(int x){
    return x = 100;
}

int main(){
    std::cout << add(1);//此处 add(1) 有二义性，编译器会报错
    return 0;
}
~~~



