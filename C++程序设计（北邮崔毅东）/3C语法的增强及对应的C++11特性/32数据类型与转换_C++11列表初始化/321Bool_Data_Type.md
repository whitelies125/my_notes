# 321 Bool Data Type

bool expression：一段代码，其结果为真（TRUE）或假（FALSE）。

建议，对于 bool 类型变量/函数，其命名最好使用 is 开头，如 isMyBook，isBoy()。其他情况也最好使用如 canWork()，hasLicense() 这样的含义显然的形式。

#### std::boolalpha

例子：

~~~C++
int main() {
    bool a = true;
    bool b = false;
    cout << "a = " << a << " b = " << b << "\n";
    cout << std::boolalpha << "a = " << a << " b = " << b << "\n";
    //std::boolalpha 的作用是
	//使后续 cout 输出时将后面的 bool 类型变量以字符 true、false 的形式输出，
    //而非以整数 1、0 的形式输出。
    return 0;
}
//运行结果
//a = 1 b = 0
//a = true b = false
//a = true b = false
~~~

输出形式改回来的例子：

~~~C++
int main() {
    bool a = true;
    bool b = false;
    cout << "a = " << a << " b = " << b << "\n";
    cout << std::boolalpha;//使后续 cout 将 bool 类型变量以字符 true、false 形式输出
    cout << "a = " << a << " b = " << b << "\n";
    cout << "a = " << a << " b = " << b << "\n";
    cout << std::noboolalpha;//使后续 cout 将 bool 类型变量以整数 1、0 形式输出
    cout << "a = " << a << " b = " << b;
    return 0;
}
//运行结果：
//a = 1 b = 0
//a = true b = false
//a = true b = false
//a = 1 b = 0
~~~

