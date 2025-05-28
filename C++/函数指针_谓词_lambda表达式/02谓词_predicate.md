## 谓词 predicate

很多算法都会比较输入序列中的元素，默认情况下，这些算法使用元素类型的 < 或 == 运算符来完成比较。
标准库还为这些算法提供了提供了额外的版本，允许我们提供自己定义的操作，来代替默认运算符。

例如，sort 算法默认使用元素类型的 < 运算符。
而可能我们希望的排序顺序与 < 所定义的顺序不同，或我们输入的序列其元素类型是未定义 < 运算符的类型。
这两种情况下，我们需要修改 sort 的默认行为。

实际上，标准库提供了 sort 的重载函数，该重载函数接收第三个参数，此参数是一个**谓词**。

## 谓词

谓词是一个可调用的表达式，其返回结果是一个能用做条件的值。

标准库所使用的谓词分为两类：

- **一元谓词 unary predicate**：只接受单一参数。
- **二元谓词 binary predicate**：有两个参数。

接收谓词函数的算法，对输入序列中的元素调用谓词。

例子：

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

bool comp(int a, int b){ // 以此实现降序排序
    return a > b ? 1 : 0;
}

int main(){
    std::vector<int> vec{1,3,5,2,4};
    
    // sort() 默认使用元素类型的 < 运算符
    sort(vec.begin(), vec.end());
    for(auto& it : vec) std::cout << it << " ";
    std::cout << std::endl;

    // sort() 的重载函数，接收二元谓词，对输入序列中的元素调用谓词
    sort(vec.begin(), vec.end(), comp);
    for(auto& it : vec) std::cout << it << " ";
    std::cout << std::endl;
    
    return 0;
}
//output:
//1 2 3 4 5 
//5 4 3 2 1
~~~

2022.08.13