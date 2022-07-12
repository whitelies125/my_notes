# 332 Automatic Type Deduction: auto

自动类型推导：auto 关键字。

auto 类型，声明变量时根据变量初始值的类型自动为此变量选择匹配的类型。

## 一. auto 使用限制

#### 1.1 auto 变量必须在定义时初始化：

~~~C++
auto a = 10;
//正确，编译器可推导出 a 为 int

auto b;
b = 10;
//错误，编译器无法推导出 b 的类型
~~~

#### 1.2 定义在一个 auto 序列的变量必须始终推导为同一类型：

~~~C++
auto a = 10, b{20};
//正确

auto c{10},d = 20.0;
//错误，编译器会报错。
~~~

#### 1.3 如果初始化表达式是引用或 const，则去除引用或 const 语义：

~~~C++
int a{10};
int& b = a;
auto c = b;
//则 c 的类型为 int 而非 int&

const int a = 10;
auto b = a;
//则 b 的类型为 int，而非 const int
~~~

#### 1.4 若 auto 关键字带上 &，则不会去除引用或 const 语义：

~~~c++
int a{10};
int& b = a;
auto& c = b;
//则 c 的类型为 int&

const int a = 10;
auto& b = a;
//则 b 的类型为const int
~~~

#### 1.5 若初始化表达式为数组时，auto 关键字推导类型为指针：

~~~C++
int a[3] = {1,2,3};
auto b = a;//此处数组名 a 退化为了指针
//则 b 的类型为 int*
~~~

#### 1.6 若表达式为数组且 auto 带上 & 时，则推导类型为数组类型：

~~~C++
int a[3] = {1,2,3};
auto& b = a;
//则 b 的类型为数组类型;
~~~

#### 1.7 在 C++14 中，auto 可作为函数的返回类型和参数类型。

~~~C++
auto max(int x, int y){
    return x > y ? x : y;
}
int main(){
    int x = 1;
    int y = 2;
    cout << max(x,y);
    return 0;
}
//输出结果：
//2
~~~

