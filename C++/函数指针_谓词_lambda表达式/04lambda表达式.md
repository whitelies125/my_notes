# lambda 表达式

[TOC]

## lambda 表达式是函数对象

当我们写了一个 lambda 表达式后，编译器将该表达式翻译成一个**未命名类的未命名对象**。

同样的，在 lambda 表达式产生的类中，含有一个重载的函数调用运算符 ()（**函数对象类**）。
根据捕获列表中的内容，在 lambda 表达式产生的类中，也可能存在的数据成员（**含有状态的函数对象类**）。

lambda 表达式产生的类不含默认构造函数、赋值运算符及默认析构函数。
是否含有默认的拷贝/移动构造函数则通常要视捕获的数据成员类型而定。

### lambda 表达式形式

~~~C++
[capture list](parameter list) -> return type { function body}
~~~

其中，**capture list 捕获列表**，是一个 lambda 所在函数中定义的局部变量的列表。
其余与普通函数一样，分别为 **parameter list 参数列表，return type 返回类型，funciton body 函数体**。

不同的是，lambda 表达式必须使用**尾置返回**来指定返回类型。

#### 无捕获列表，无传递参数的例子

我们可以忽略参数列表和返回类型，但必须永远包含捕获列表和函数体。

~~~C++
#include <iostream>
int main(){
    auto f = [] {//lambda 表达式，省略了 (parameter list) -> return type
        return 42;
    };
    std::cout << f() << std::endl;
    return 0;
}
//output:
//42
~~~

此处，定义了一个可调用对象 f，不接受参数，返回 42。

此处，省略括号和参数列表，等价于指定一个空参数列表 ()。
此处，省略了返回类型，则根据返回的表达式的类型推断。

#### 关于返回类型推断

cppreference 上说：

> 如果不存在尾置返回类型，则其返回类型，与返回类型为 auto 的函数一样，将从 return 语句中推断得出。

「C++11 Primer」书上说：

>如果函数体只是一个 return 语句，则返回类型从返回的表达式的类型推断而来。
>否则，返回类型为 void。
>默认情况下，如果一个 lambda 函数体包含 return 指外的任何语句，则编译假定此 lambda 返回 void。

「C++11 Primer」书上给出的例子：

它说的正确的例子：

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    std::vector<int> vec {-2,-1,0,1,2};
	//正确，仅有 return 语句，从 return 表达式中推断
    transform(vec.begin(), vec.end(), vec.begin(),
              [](int i){ return i < 0 ? -i : i;});
    for_each(vec.begin(),vec.end(),
             [](int i){ std::cout << i << ' ';});
    return 0;
}
//output:
//43
~~~

它说的错误，会编译出错的例子：

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    std::vector<int> vec {-2,-1,0,1,2};
	//错误，没有指明返回类型，默认返回 void，不能返回值，会编译出错
    //应该加上尾置返回类型 -> int 才正确
    transform(vec.begin(), vec.end(), vec.begin(),
        [](int i){
            if(i < 0) return -i;
            else return i;
        });
    for_each(vec.begin(),vec.end(), [](int i){ std::cout << i << ' ';});
    return 0;
}
//output:
//43
~~~

但实际上，我这里二者都通过了编译，即便我指定了 C++ 版本为 C++11：

~~~C++
g++ -std=c++11 -o main.exe main.cpp
~~~

也是都通过编译，正常运行的。

**至于为什么，暂且不明**。也许是编译器额外实现了标准以外的功能？

#### 向 lambda 传递参数

与普通函数调用类型，调用一个 lambda 表达式时给定的实参会被用来初始化 lambda 表达式的形参。

但不同的是，**lambda 不能有默认参数**，而普通函数可以有默认参数如 func(int a, int b = 1)。
因此，一个 lambda 调用的实参数目永远与形参数目相等。

一旦形参初始化完毕，就可以执行函数体了。

在 [02谓词_predicate](02谓词_predicate.md) 中向 sort() 算法传递函数的方式，现在可以换用 lambda 表达式：

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>
int main(){
    std::vector<int> vec{1,3,5,2,4};
    // 使用 lambda 表达式，实现降序排序
    sort(vec.begin(), vec.end(),
        [](int a, int b) -> bool{
            return a > b ? 1 : 0;
        });
    for(auto& it : vec) std::cout << it << " ";
    std::cout << std::endl;
    return 0;
}
//output:
//5 4 3 2 1
~~~

此处捕获列表 [] 为空，表示不使用 lambda 表达式所在函数中的任何局部变量。

#### 使用捕获列表

lambda 可以出现在一个函数中，并使用该函数的局部变量，但 lambda 需要将要使用的变量在捕获列表中指明。
lambda 通过将所在函数的局部变量包含在其捕获列表中，来指出将会使用这些变量。
捕获列表指引 lambda 在其内部访问局部变量所需的信息。

例如：

~~~C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
int main(){
    std::vector<std::string> words {"ab", "cde", "fg"};
    int size = 3;
    auto wc = find_if(words.begin(), words.end(),
        [size] (const std::string& str) {//在捕获列表中指出，捕获 size
            return str.size() >= size;//方可在函数体中使用该局部变量 size
            });
    std::cout << *wc << std::endl;
    return 0;
}
//output:
//cde
~~~

在这个例子中，我们在捕获列表中指出了捕获 lambda 所处函数 main() 中的局部变量 size，才可在 lambda 函数体中使用变量 size。

但实际上，该例中，lambda 函数体中的变量 size，并非 main() 函数中的变量 size。详见下文可知。

### lambda 表达式产生的类

#### 捕获列表为空产生的类

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

int main(){
    std::vector<int> vec{1,3,5,2,4};
    // 使用 lambda 表达式，实现降序排序
    sort(vec.begin(), vec.end(),
        [](int a, int b) -> bool{
            return a > b ? 1 : 0;
        });
    for(auto& it : vec) std::cout << it << " ";
    std::cout << std::endl;
    return 0;
}
//output:
//5 4 3 2 1
~~~

其行为类似于下面这个类的一个未命名对象：

~~~C++
class Comp{
public:
    bool operator() (int a, int b) const { //注意，这里有 const
        return a > b ? 1 : 0;
    }
};
~~~

产生的类只有一个函数调用运算符成员，其形参列表和函数体与 lambda 表达式完全一样。
注意，在默认情况下，lambda 不能改变它捕获的变量，也就是说，在默认情况下，由 lambda 产生的类，当中的函数调用运算符 () 是一个 **const 成员函数**，不修改其成员变量。

可采用使用该类，而不使用 lambda 表达式，达到同样的效果：

~~~C++
#include <iostream>
#include <vector>
#include <algorithm>

class Comp{
public:
    bool operator() (int a, int b) const { //默认为 const 成员函数
        return a > b ? 1 : 0;
    }
};

int main(){
    std::vector<int> vec{1,3,5,2,4};
    //使用 Comp 类，而不使用 lambda 表达式，达到同样的效果，实现降序排序
    sort(vec.begin(), vec.end(),Comp());
    for(auto& it : vec) std::cout << it << " ";
    std::cout << std::endl;
    return 0;
}
//output:
//5 4 3 2 1
~~~

#### 捕获列表非空产生的类

##### 值捕获

~~~C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(){
    std::vector<std::string> words {"ab", "cde", "fg"};
    int size = 3;
    auto wc = find_if(words.begin(), words.end(),
        [size] (const std::string& str) {
            return str.size() >= size;
            });
    std::cout << *wc << std::endl;
    return 0;
}
//output:
//cde
~~~

该 lambda 表达式产生的类将形如：

~~~C++
class CompSize{
private:
    int size;//该数据成员通过值捕获对象
public:
    CompSize(int n) : size(n) {}//有构造函数
    bool operator() (const std::string& str) const {// 默认为 const
        return str.size() >= size;
    }
};
~~~

使用该类，而不使用 lambda，可达到同样效果：

~~~C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

class CompSize{
private:
    int size;//该数据成员通过值捕获对象
public:
    CompSize(int n) : size(n) {}//有构造函数
    bool operator() (const std::string& str) const {// 默认为 const
        return str.size() >= size;
    }
};

int main(){
    std::vector<std::string> words {"ab", "cde", "fg"};
    int size = 3;
	//使用 CompSize 类，而不使用 lambda 表达式，达到同样的效果
    auto wc = find_if(words.begin(), words.end(),CompSize(size));
    std::cout << *wc << std::endl;
    return 0;
}
//output:
//cde
~~~

##### 引用捕获

「C++11 Primer」书说：

> 对于引用捕获变量，编译器可直接使用该引用而无需再 lambda 产生的类中将其存储为数据成员。

我对这句话的理解就是，lambda 产生的类中没有引用捕获的数据成员，仅有值捕获的数据成员。

但我找到 https://eel.is/c++draft/expr.prim.lambda 中写到：

> An entity is captured by reference if it is implicitly or explicitly captured but not captured by copy[.](https://eel.is/c++draft/expr.prim.lambda#capture-12.sentence-1)
>
> It is unspecified whether additional unnamed non-static data members are declared in the closure type for entities captured by reference[.](https://eel.is/c++draft/expr.prim.lambda#capture-12.sentence-2)
>
> If declared, such non-static data members shall be of literal type.

一个被显式或隐式引用捕获，非值捕获的实体。
是否在闭包类型（lambda 产生的类）中额外地声明未命名非静态数据成员，是未明确的规定的。
若声明，则这些非静态数据成员应当是 literal type。

所以，对于引用捕获：

~~~C++
#include <iostream>
#include <string>
int main(){
    std::string str = "abc"; //局部变量
	//引用传递，可调用对象 f 中的数据成员 str 为局部变量 str 的引用
    auto f = [&str] { return str; };
    str = "bcd";//str = 0 修改局部变量的 str 中的 str 也会改变值
    auto j = f();//j = 0
    std::cout << str << std::endl;
    std::cout << j << std::endl;
    j = "cde";
    std::cout << str << std::endl;
    std::cout << j << std::endl;
    return 0;
}
//output
//bcd
//bcd
//bcd
//cde
~~~

是否产生的类，如下所示，在类中有引用捕获对应的数据成员：

~~~C++
class CapturedByReference{
private:
	std::string& str;//在类中有引用捕获对应的数据成员
public:
    CapturedByReference(string& str) : str(str) {}
	std::string operator() (){
        return str;
    }
}
~~~

亦或没有，而是编译器以其它方式实现。

目前，我就不清楚了。
但其对外表现，应当是与这个类的对外表现一致的。

## lambda 的捕获

#### 值捕获

~~~C++
#include <iostream>

int main(){
    int val = 42; //局部变量
    auto f = [val] { return val; };//值传递，拷贝给可调用对象 f 中的数据成员 val
    val = 0;//修改局部变量的 val，而不会修改 f 中的 val
    auto j = f();//j = 42
    std::cout << val << std::endl;
    std::cout << j << std::endl;
    return 0;
}
//output
//0
//42
~~~

#### 引用捕获

~~~C++
#include <iostream>

int main(){
    int val = 42; //局部变量
	//引用传递，可调用对象 f 中的数据成员 val 为局部变量 val 的引用
    auto f = [&val] { return val; };
    val = 0;//val = 0 修改局部变量的 val，f 中的 val 也会改变值
    auto j = f();//j = 0
    std::cout << val << std::endl;
    std::cout << j << std::endl;
    return 0;
}
//output
//0
//0
~~~

#### 隐式捕获

除了显式地在捕获列表指出我们想要使用的所在函数中的局部变量，还可以使编译器根据 lambda 函数体中的代码来推断我们要使用哪些变量。

但为了指示编译器推断捕获列表，还需在捕获列表中写 = 或 &，来告诉编译器采用值捕获或引用捕获方式。

~~~C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(){
    std::vector<std::string> words {"ab", "cde", "fg"};
    int size = 3;
    auto wc = find_if(words.begin(), words.end(),
        [=] (const std::string& str) {// 隐式捕获，值捕获
            return str.size() >= size;//编译器根据函数体，自动推断出需要捕获 size
            });
    std::cout << *wc << std::endl;
    return 0;
}
//output:
//cde
~~~

若希望对一部分变量使用值捕获，另一部分变量使用引用捕获，则可混合使用显示捕获和隐式捕获。

注意，当混合使用显示捕获和隐式捕获时：

- 此时捕获列表中的第一个元素必须是 = 或 &。
- 显示捕获的变量必须与隐式捕获采用不同的方式，即隐式捕获为 =，显式捕获必须使用 &，反之亦然。

~~~C++
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

int main(){
    std::vector<std::string> words {"ab", "cde", "fg"};
    std::ostream& os = std::cout;
    char seq = ',';
    // seq 值捕获；其它由编译器推断采用 引用捕获
    for_each(words.begin(), words.end(),
        [&, seq](const std::string& str){//此时捕获列表中的第一个元素必须是 = 或 &。
            os << str << seq;
        });
    // os 引用捕获；其它由编译器推断采用 值捕获
    for_each(words.begin(), words.end(),
        [=, &os](const std::string& str){//此时捕获列表中的第一个元素必须是 = 或 &。
            os << str << seq;
        });
    return 0;
}
//output:
//ab,cde,fg,ab,cde,fg,
~~~

#### 可变 lambda

如前面所说，lambda 生成的类，其成员函数被 const 所修饰，无法修改数据成员。

实际上是无法修改值捕获的变量，引用捕获的是可以修改的。
和类的数据成员一样的，虽然 const 修饰的成员函数，无法修改数据成员，但是引用和指针类似，我们并没有改指针指向的对象，而是改的指向对象的值。
所以对引用捕获的变量是可以直接修改无需加 mutable 的。

但如果在一些情况下，我们希望能够改变被值捕获的变量的值，则可以在参数列表首加上关键字 mutable，其作用相当于取消成员函数的 const 修饰，这样即可修改 lambda 中值捕获到的变量。

~~~C++
#include <iostream>
#include <algorithm>
int main(){
    int val = 42;
	//默认为 const，不能修改数据成员 val
    //编译会报错：
    //main.cpp: In lambda function:
    //main.cpp:7:29: error: increment of read-only variable 'val'
    //    7 |     auto f = [=] { return ++val; };
    //      |                             ^~~
    auto f = [=] { return ++val; };
    val = 0;
    auto j = f();
    std::cout << j << std::endl;
    return 0;
}
~~~

因为生成的类：

~~~C++
class CapturedByValue{
    private:
    int val;
    CapturedByValue(int val) : val(val) {}
	int operator() () const { // const 函数，不能修改成员变量。
        return ++val; 
    }
}
~~~

加上 mutable：

~~~C++
#include <iostream>
#include <algorithm>
int main(){
    int val = 42;
    //编译有 warning，但能正常运行：
    //main.cpp: In function 'int main()':
    //main.cpp:7:18: warning: parameter declaration before lambda declaration specifiers only optional with '-std=c++2b' or '-std=gnu++2b'
    //    7 |     auto f = [=] mutable { return ++val; };
    //      |                  ^~~~~~~
    //改成 g++ -std=c++2b -o main.exe main.cpp，就没有 warning 了
    auto f = [=] mutable { return ++val; };
    val = 0;// 局部变量为 0
    auto j = f();// j 为 43
    std::cout << val << std::endl;
    std::cout << j << std::endl;
    return 0;
}
//output:
//0
//43
~~~

这样生成的类为：

~~~C++
class CapturedByValue{
    private:
    int val;
    CapturedByValue(int val) : val(val) {}
	int operator() () { // 去除了 const，可修改成员变量
        return ++val; 
    }
}
~~~

对于引用捕获的变量，实测可以直接修改，无需写 mutable：

~~~C++
#include <iostream>
#include <algorithm>
int main(){
    int val = 42;
    auto f = [&] { return ++val; };
    val = 0;
    auto j = f();
    std::cout << val << std::endl;
    std::cout << j << std::endl;
    return 0;
}
//output
//1
//1
~~~

生成的类：

~~~C++
class CapturedByValue{
    private:
    int& val;
    CapturedByValue(int& val) : val(val) {}
	int operator() () const { //虽然是 const，但我们也没改 val 为另一个对象的引用是吧
        return ++val; 
    }
}
~~~

