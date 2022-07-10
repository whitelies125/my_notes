# Template Basics

初识模板。

## Why Template?

举例：求两个整数，或双精度浮点数，或字符的最大者。

在 C 语言中，需要 3 个不同名函数。

~~~C
int max_int(int a, int b);
doule max_doule(double a, double b);
char max_char(char a, char b);
~~~

在 C++ 中，需要 3 个重载函数。

~~~C++
int max(int a, int b);
doule max(double a, double b);
char max(char a, char b);
~~~

注意到，实际上这 3 个函数，其逻辑相同的，函数体内的代码也相同。

## Function with generic type

带有泛型的函数。

如果可以用一个东西来表示此处为一个未定的数据类型，但在调用时再根据我们传入的参数，确定这个东西表示的确定的数据类型，就好啦。

~~~C++
GenericType max(GenericType a, GenericType b){
    return a > b ? a : b;
}
~~~

这样我们就只用写这 1 个函数，但仍然可以在使用：

~~~C++
int main(){
    std::cout << max(1,2);
    std::cout << max(1.0,2.0);
    std::cout << max('a','b');
    return 0;
}
~~~

这就是泛型。

