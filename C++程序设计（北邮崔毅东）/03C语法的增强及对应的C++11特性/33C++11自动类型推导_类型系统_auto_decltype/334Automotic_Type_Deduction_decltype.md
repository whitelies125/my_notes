# 335 Automatic Type Deduction: decltype

自动类型推导：decltype 关键字。

declare+type = decltype

decltype 在编译时期推导一个表达式的类型，而不用初始化。

~~~C++
int func1(){ return 10;}
auto func2(){ return 'g';}//C++14

int main() {
    decltype(func1()) x;
	//x 的类型与 func1() 的返回值类型相同
	//比较，这里的 x 并不像使用 auto 必须初始化。
    decltype(func2()) y = func2();//y 的类型与 func2() 的返回值类型相同
    return 0;
}
~~~

方便之处，即便将 func2() 的返回值改为整数 4，其他地方也无需改动。

decltype 主要用于泛型编程（模板）。