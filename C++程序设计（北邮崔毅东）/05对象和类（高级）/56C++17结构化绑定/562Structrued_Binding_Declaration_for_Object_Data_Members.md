# Structured Binding Declaration for Object Data Members

用于对象数据成员的结构化绑定声明。

若初始化表达式为类/结构体类型，则标识符列表中的名字绑定到类/结构体的非静数数据成员上。

- 数据成员必须为公有成员。
- 标识符数量必须等于数据成员的数量。
- 标识符类型与数据成员类型一致。

~~~C++
class C{//也可用 struct C，然后去掉 public 属性说明。
    	//struct 是默认 public 的,class 默认private
public:
    int i {420};// 就地初始化
    char ca[3] {'O', 'K', '!'};// 就地初始化
    
};
int main(){
    C c;
    auto [a1, a2] {c};//a1 是 int 类型，a2 是 char[] 类型
    std::cout << a1 << std::endl;
    std::cout << a2 << std::endl;
    return 0;
}
//output:
//420
//OK!
~~~

## More Examples

类似的：

auto 后跟 &，表示标识符是数据成员的引用。
auto 前可放置 const，表示标识符是只读的。

~~~C++
#include <iostream>
class C{
public:
    int i {420};
    char ca[3] {'O', 'K', '!'};
};
int main(){
    C c;
    auto [a1, a2] {c};

    a1 = 100;
    std::cout << a1 << std::endl;
    std::cout << c.i << std::endl;

    auto &[b1, b2] {c};//b1 是 int& 类型，b2 是 char & [3]类型（数组的引用），是 c.ca 的引用。
    b1 = 200;
    std::cout << b1 << std::endl;
    std::cout << c.i << std::endl;
    return 0;
}
//output:
//100
//420
//200
//200
~~~

