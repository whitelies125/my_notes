# 371 Range-based for-loop

~~~C++
for( 元素名变量 : 广义集合){
    循环体
}
~~~

元素名变量：
可以是引用类型，以便直接修改集合元素的值。
可以是 const 类型，以避免修改集合元素的值。

广义集合：
就是 Range 范围，是一些元素组成的一个整体。
例如：

~~~C++
auto  a1[] { 1, 3, 5, 7 };
std::array<int, 4> a2 { 2, 4, 6, 8};
std::vector<int> v = { 42, 7, 5 };
std::vector<std::string> s {"Hello", "World", "!"};
~~~

而如果仅仅是一个单纯的指针，是无法使用基于范围的 for 循环的。

用例：

~~~C++
int a[] = { 2,1,4,3,5 };
for (auto i : a){
    std::cout << i << std::endl;、
}
for (auto& i : a){
    i = 2 * i;
}
~~~

限制：
基于范围的循环仅限于 for 语句。
do...while(); 和 while(){}; 不支持基于范围的循环。

