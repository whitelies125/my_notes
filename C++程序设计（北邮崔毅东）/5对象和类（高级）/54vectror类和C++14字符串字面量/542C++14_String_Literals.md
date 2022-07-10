# C++14: String Literals

C++14：字符串字面量。

## C++11 Raw String literals

C++11 原始/生 字符串字面量。

语法：

~~~C++
R"delimiter(raw_characters)delimiter"
~~~

delimiter：定界符。

如：

~~~C++
#include <iostream>
const char* s1 =R"(Hello
world)";
//下列 s2,s3 都与 s1 效果相同。
const char* s2 ="Hello\nworld";
const char* s3 =R"NoUse(Hello
world)NoUse";

int main(){
    std::cout << s1 << std::endl;
    std::cout << s2 << std::endl;
    std::cout << s3 << std::endl;
    return 0;
}
//输出结果：
//Hello
//World
//Hello
//World
//Hello
//World
~~~

## C++14 将 ""s 进行了重载

~~~C++
using namespace std::string_literals;//使用 ""s 需要写上这句，下面 "Hello!"s 才不会报错。
auto hello = "Hello!"s;//hello is of std::string type
auto hello = std::string{"Hello!"};//equals to the above

auto hello = "Hello!";//hello is const char* type
~~~

这里老师讲得也太少，太模糊了。

下面是自己的尝试：

首先，使用

~~~C++
#include <typeinfo>//头文件
typeid(...).name();
~~~

可以查看类型，但在 g++ 中使用，会有一点点小问题：

~~~C++
#include <iostream>
#include <typeinfo>

int main(){
    std::cout << "int : " << typeid(int).name() << std::endl;
    std::cout << "double : " << typeid(double).name() << std::endl;
    std::cout << "char : " << typeid(char).name() << std::endl;
    std::cout << "std::string : " << typeid(std::string).name() << std::endl;
    return 0;
}
//输出结果为：
//int : i
//double : d
//char : c
//std::string : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
~~~

可见，编译器为 g++ 时，typeid(...).name() 返回的不是我们常见的 int，char，而是其它的不太看得懂的字符串。
不过，看网上信息，至少是一一对应的。

所以，在这里先输出 const char * 和 std::string 类型对于的字符串：

~~~C++
#include <iostream>
#include <typeinfo>

int main(){
    std::cout << "const char * : " << typeid(const char *).name() << std::endl;
    std::cout << "std::string : " << typeid(std::string).name() << std::endl;
    return 0;
}
//输出：
//const char * : PKc
//std::string : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
~~~

然后是测试：

~~~C++
#include <iostream>
#include <typeinfo>
#include <string>
int main(){
    std::cout << "const char * : " << typeid(const char *).name() << std::endl;
    std::cout << "std::string : " << typeid(std::string).name() << std::endl;

    using namespace std::string_literals;//需要写上这句，下面的 "Hello!"s 才不会报错。
    auto hello_1 = "Hello!"s;//hello is of std::string type
    auto hello_2 = std::string{"Hello!"};//equals to the above
    auto hello_3 = "Hello!";//hello is const char* type

    std::cout << "hello_1 type : " << typeid(hello_1).name() << std::endl;
    std::cout << "hello_2 type : " << typeid(hello_2).name() << std::endl;
    std::cout << "hello_3 type : " << typeid(hello_3).name() << std::endl;
    return 0;
}
//输出：
//const char * : PKc
//std::string : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//hello_1 type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//hello_2 type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//hello_3 type : PKc
~~~

可见，的确如老师所说。

然后是老师举的第二个例子。

~~~C++
#include <iostream>
#include <string>
#include <typeinfo>

int main(){
    std::cout << "const char * : " << typeid(const char*).name() << std::endl;
    std::cout << "std::string : " << typeid(std::string).name() << std::endl;

    using namespace std::string_literals;
    std::string s1 = "abc\0\0def";
    std::string s2 = "abc\0\0def"s;
    std::cout << "s1.size : " << s1.size() << std::endl
            << "s1 content : "<< " \"" << s1 << "\"" << std::endl
            << "s2 type : " << typeid(s1).name() << std::endl;
    std::cout << "s2.size : " << s2.size() << std::endl
            << "s2 content : "<< " \"" << s2 << "\"" << std::endl
            << "s2 type : " << typeid(s2).name() << std::endl;
    return 0;
}
//输出：
//const char * : PKc
//std::string : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//s1.size : 3
//s1 content :  "abc"
//s2 type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//s2.size : 8
//s2 content :  "abcdef"
//s2 type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
~~~

显然的是，s1 和 s2 都是 std::string 类型。

但区别在于 s1.size() 仅为 3 ，而 s2.size() 为 8。
这里其实可以猜测，是因为 \0 的原	因，导致 s1 计算长度时遇到 \0 截止，从而返回的是 3。

~~~C++
#include <iostream>
#include <string>
#include <typeinfo>

int main(){
    using namespace std::string_literals;
    std::cout << "std::string : " << typeid(std::string).name() << std::endl;
    const char c[9] = "01234567";
    std::cout << "char c[9] type : " << typeid(c).name() << std::endl;

    std::cout << "\"abc\\0\\0def\" type : " << typeid("abc\0\0def").name() << std::endl;
    std::cout << "\"abc\\0\\0def\"s type : " << typeid("abc\0\0def"s).name() << std::endl;
    return 0;
}
//输出：
//std::string : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
//char c[9] type : A9_c
//"abc\0\0def" type : A9_c
//"abc\0\0def"s type : NSt7__cxx1112basic_stringIcSt11char_traitsIcESaIcEEE
~~~

突然发现，"abc\0\0def" 是 char [] 类型，而 "abc\0\0def"s 是 std::string 类型。

结合我的知识，我这里大胆猜想：

std::string 是一个类，当创建其对象时，会调用其构筑函数之类的。而在函数传参时，数组作为传递的参数时，会退化为指针。于是，无论是用 const char * 还是 char c[] 创建 std::string 对象，都仅仅是一个指针而不具有长度信息，所以以 /0 为结束创建的 std::string 对象。

而 "abc\0\0def"s 本身就是 std::string 类型，这里应该属于拷贝初始化。

这也就是为什么，s1.size() 为 3，而 s2.size() 为 8。

所以 ""s 的作用，就是使得我们写什么，std::string 对象里面存的就是什么。
应该是这样吧。

这里老师讲得也非常模糊，一带而过。
我暂且也只能自己摸索到这里了。