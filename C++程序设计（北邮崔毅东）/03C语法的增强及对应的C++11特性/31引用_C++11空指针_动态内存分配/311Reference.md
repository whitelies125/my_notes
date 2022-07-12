# 311 Reference

GCC 编译器是 C++ 程序所使用的 3 种主流编译器之一，其余两种为 msvc 和 clang。

## 一. 引用类型

### 1.1 引用的声明：

~~~c++
int x;
int& rx = x;
~~~

或：

~~~C++
int x, & rx = x;
~~~

引用就是另一个变量的别名。
对引用进行的操作，实际是作用在原变量上。

个人感觉就是因为指针太自由了，于是对指针添加了一些限制后，就有了引用。

### 1.2 特点

#### 1.2.1 引用必须初始化	

~~~C++
int main(){
    int x = 0;
    int& rx = x;
    cout << "x = " << x << ",rx = " << rx;
    return 0;
}
//上述写法是正确的。

int main(){
    int& rx;
    int x = 0;
    rx = x;
    cout << "x = " << x << ",rx = " << rx;
    return 0;
}
//这样写是错误的。
//错误提示：error: 'rx' declared as reference but not initialized（rx 作为引用类型声明，但未初始化）
~~~

此处也可见，引用与指针的一点区别。

引用必须初始化，意味着引用必定是另一个变量的别名（指向另一变量），而指针是可以为空指针的，引用不能。

#### 1.2.2 引用与初始值绑定，无法更改，无法重新绑定另一对象

~~~C++
int main(){
    int x = 0;
    int y = 1;
    int& rx = x;
    rx = y;
    cout << "x = " << x << ",rx = " << rx;
    return 0;
}
//运行结果：
//x = 1,rx = 1
~~~

本质是使 x = y。毕竟 rx 就是 x 的别名。
别误以为是使 rx 重新变为 y 的引用了，不是的。

若将 rx = y 改为 &rx = y，则会报错：error: value required as left operand of assignment。

### 1.3 函数参数

#### 1.3.1 引用可作函数形参，调用时实参只需传递普通变量，不用传递地址

对比：

以指针为形参：

~~~C++
void swap(int *x, int *y){//以指针为形参
    int  t;
    t = *x;
    *x = *y;
    *y = t;
}
int main() {
    int a = 5;
    int b = 10;
    cout << "Before: a = " << a << ",b = " << b << "\n";
    swap(&a,&b);//需传递地址，才能交换 a,b 的值。
    cout << "After: a = " << a << ",b = " << b << "\n";
    return 0;
}
~~~

以引用为形参：

~~~C++
void swap(int& x, int& y){//以引用为形参
    t = x;
    x = y;
    y = t;
}
int main() {
    int a = 5;
    int b = 10;
    cout << "Before: a = " << a << ",b = " << b << "\n";
    swap(a,b);//直接传递 a,b 就好啦。
    cout << "After: a = " << a << ",b = " << b << "\n";
    return 0;
}
~~~

#### 1.3.2 在被调函数中改变引用变量的值，实际是改变的是实参的值

例子就是以引用为形参的例子。