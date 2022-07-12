# Type Conversion

C 风格的类型转换：

~~~c
printf("%d",(int)2.5);
printf("%d",int(2.5));
~~~

C++ 风格的类型转换：

~~~C++
static_cast<类型名>(变量/表达式)
~~~

简单的例子：

~~~c++
int main() {
    double x = 2.5;
    cout << x << "\n";
    cout << static_cast<int>(x);
    return 0;
}
//运行结果：
//2.5
//2
~~~

同样是崔毅东推荐的代码规范：

>类型转换必须显示声明，永远不要依赖隐式转换。

显式声明，就是上面的方式。
隐式转换是指如 int x = 1.1 这样的。
