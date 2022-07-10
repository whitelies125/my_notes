# 361 Scope Of Variables

变量作用域

## Scope of Local Variables 局部变量作用域

从定义位置开始，到块（block）的结束。

## Scope of Local Variables in Embedded/Non-embedded Blocks 嵌套/非嵌套块中的局部变量作用域。

It is fine to declare "i" in 2 non-nesting blocks. 

~~~c++
void function(){
    for(int i = 1; i < 5; i++){
        ...
    }
    for(int i = 1; i < 10; i++){
        ...
    }
}
~~~

It is bad to declare "i" in 2 nesting blocks. 

~~~C++
void function(){
    int i = 1;
    for(int i = 1; i < 10; i++){//not good,
        ...
    }
    cout << i;
}
~~~

## Unary Scope Resolution 一元作用域解析符

If a local variable name is the same as a global variable name, you can access the global variable using **::globalVariable**. 

当局部变量名与全局变量名相同时，可以使用 :: 访问**全局变量**。
注意 :: 用于访问的是全局变量，而非比该局部变量名所在作用域，更大范围的作用域里的同名局部变量。

The :: operator is known as the **unary scope resolution**.

~~~C++
#include <iostream>

int x = 10;

int main(){
    int x = 5;
    std::cout << x << std::endl;//输出结果为 5
    std::cout << ::x << std::endl;//输出结果为 10
    return 0;
}
~~~

