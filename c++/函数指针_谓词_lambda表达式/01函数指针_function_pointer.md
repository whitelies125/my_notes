# 函数指针 function pointer

#### 函数指针的声明

例如，一个常见的函数声明为：

~~~C++
int func(int a, char b);//常见的函数声明
~~~

只需将函数名以带括号的函数指针名替换掉：

~~~C++
int (*pf)(int, char);//函数指针的声明
~~~

便是一个函数指针的声明。

此处的函数指针，是一个变量名为 pf，指向**返回值类型为 int，接收参数列表类型为 int，char 的函数**的指针。

当然，函数指针也可为空，表示该指针没有指向任何一个函数。

注意，这里 (pf) 的括号 () 是不可少的，否则：

~~~C++
int *pf(int, char);//会识别为函数
~~~

这是声明了一个返回值类型为 int*，名为 pf 的函数。

#### 函数指针的赋值

将函数名当作一个值来使用时，该函数名自动转换为函数指针。

~~~C++
pf = func;//func 会自动转换为函数指针类型
pf = &func;//手动转换
~~~

当然，函数指针与其指向的函数，二者的返回值类型、形参类型需相匹配。

#### 通过函数指针调用函数

可以通过使用函数指针调用函数，且可无需使用解引用符。

~~~C++
pf(1, 'a');//可无需写解引用符号
(*pf)(1, 'a');//写解引用符也可以
func(1, 'a');//与上面的方式等价
~~~

一个简单的例子：

~~~C++
#include <iostream>

void func(int a, char b){
    std::cout << a << " " << b << std::endl;
}

int main(){
    void (*p)(int, char);
    p = &func;//p = fun; 也可
    //当然也可以声明并初始化，一行完成
    //void (p)(int, char) = func; 
    //void (p)(int, char) = &func;
    //都可

    p(1, 'a');
    (*p)(1, 'a');
    p(2.1, 'a');//测试一下传入 double，隐式转换 int，实测可行
	(*p)(2.1, 'a');//那感觉确实和正常函数用起来一样的。
    
    return 0;
}
//output:
//1 a
//1 a
//2 a
//2 a
~~~

#### 重载函数的函数指针

重载函数的函数指针，需明确地与重载函数中地某一个函数精确匹配。

~~~C++
void func_overload(int);
void func_overload(int, char);
~~~

则指针：

~~~C++
void (*pf_overload)(int) = func_overload;//正确
void (*pf_overload)(int, char) = func_overload;//正确
void (*pf_overload)(int*) = func_overload;//错误，没有相匹配的函数
double (*pf_overload)(int) = func_overload;//错误，没有相匹配的函数
~~~

#### 函数指针作为形参

格式：

~~~C++
void func_para(bool a, double b, bool pf(int, char));
//虽然 bool pf(int, char) 是函数类型，但类似传递数组，会自动转换为函数指针类型。
void func_para(bool a, double b, bool (*pf)(int, char));
//或者直接显式地将形参写为函数指针类型，也可
~~~

调用函数时，可以直接将函数名作为实参使用，此时会自动转换为指针。

~~~C++
func_para(1, 'a', func);
~~~

#### typedef 简化

写函数指针类型太长了，所以使用 typedef 起个简短的别名。

~~~C++
//这里的 func 就是前文的 int func(int a, char b);
typedef Fun(int ,char);//Fun 是函数类型，或
typedef decltype(func) Fun2;//Fun2 也是函数类型，二者等价

typedef bool (FuncP)(int ,char);//FuncP 是指向函数的指针，或
typedef decltype(func) FuncP2;//FuncP2 也是指向函数的指针，二者等价
~~~

所以，前文所述的函数指针作为形参可以改写为：

~~~C++
void func_para(bool a, double b, Fun);//函数类型自动转换为函数指针类型
void func_para(bool a, double b, FuncP2);//直接为为函数指针类型
~~~

#### 返回指向函数的指针

函数无法返回一个函数，但能返回一个函数指针。

对于函数的返回值类型，如果是函数类型，编译器并不会将函数类型自动转换为指针类型，所以我们的返回类型必须写为指针类型，而非如前文一样往往二者皆可。

比较方便的做法是先给要返回的函数指针类型起一个别名：

~~~C++
using F = int(int*, int);//F 为函数类型，不是指针
using PF = int(*)(int*, int*);//PF 为函数指针类型
~~~

然后再写返回函数指针的函数的声明：

~~~C++
PF f1(int);//正确，f1 返回 PF
F f1(int);//错误，f1 是函数，函数不能返回函数，但能返回函数指针
F* f1(int);//正确，显式指定了返回类型是指向函数的指针
~~~

等同于

~~~C++
int (*f1(int))(int*, int);
~~~

f1(int) 是函数；
(*f1(int)) 说明 f1 返回一个指针，该指针指向函数，该函数为返回值类型为 int，接收形参类型为 int\*，int 。

也可采用**尾置返回类型**的方法：

~~~C++
auto f1(int) -> int (*)(int*, int);
~~~

原返回类型处使用 auto，在函数的形参列表后使用右箭头符号 ->，其后为该函数的返回类型。