# 333 Almost Always Auto

尽量使用 auto。

使用 auto 是为了代码的正确性、性能、可维护性、健壮性以及方便。

例如 auto 强制要求了在声明变量时就初始化。

当然这是崔毅东的推荐。

将以往的初始化改为使用 auto 的形式：

~~~C++
int x = 3;
auto x = int {3}//列表初始化

int x = 42;
auto x = 42;

float x =42.;
auto x = 42.f;

unsigned long x = 42;
auto x = 42ul;

std::string x = "42";
auto x = "42"s;//C++14

chrono::nanoseconds x {42};
auto x = 42ns;//C++14

int func(double);
auto func(double)->int;//返回类型为 int
auto func(double){/*...*/};
auto func = [](double){/*...*/};//匿名函数
~~~

