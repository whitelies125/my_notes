# Function Template

函数模板。

C++ introduces function template with generic types.

C++ 引入了带有泛型的函数模板。

格式：

~~~C++
template<typename T>
returnType functionName(T para1, .../*parameter list*/){
    //Function Bady
}
~~~

其中

~~~C++
template<typename T>
~~~

称为 template prefix（模板前缀）。

所有的函数模板必须有模板前缀，没有的就不叫模板。

~~~C++
T
~~~

称为 type parameter（类型参数），T 是一个形参，不是实参。

对于一个函数模板来说，类型参数 T 在函数的形参列表中至少出现一次。

T 可以出现在 returnType，Function Bady，parameter list。

## How to specify a type parameter?

如何声明类型参数？

下面两种方式都可。

~~~C++
<typename T>//较好，描述性强
<class T>//易与类声明混淆，不推荐。
~~~

这里在网上看到一个说法，早期为了不增加关键字，所以复用了 class 这个关键字，后来 C++ 委员会觉得这样混用可能造成概念上的混淆，所以增加的 typename 这个关键字，于是 class 就保留了下来。

## Multiple type parameters

多个类型参数。

What if a function template has more one parameter?

若函数模板有多于一个类型参数该怎么处理？

- 类型参数之间使用逗号分隔。

~~~C++
template<typename T, typename S>
auto add(T x1, S x2){//C++14,auto 作为函数返回值类型
    return x1+x2;
}
~~~

## Using Function Template

使用函数模板。

~~~c++
template<typename T, typename S>
auto add(T x1, S x2){
    return x1+x2;
}
int main(){
	auto y = add(1, 2.0);
	return 0;
}
~~~

编译器根据函数调用的实参，生成函数模板的实例。

过程：

~~~c++
add(1, 2.0);
//推出
add(int, double);
//推出
T 为 int, S 为 double
//生成具体的函数
auto add(int x1, double x2){
    return x1+x2;
}
//推出 auto
double add(int x1, double x2){
    return x1+x2;
}
~~~

所以，最终调用的函数的定义为：

~~~C++
double add(int x1, double x2){
    return x1+x2;
}
~~~

也就是，编译器根据 add(1, 2.0) 生成了

~~~C++
double add(int x1, double x2){
    return x1+x2;
}
~~~

这段代码，然后编译，链接这段代码。