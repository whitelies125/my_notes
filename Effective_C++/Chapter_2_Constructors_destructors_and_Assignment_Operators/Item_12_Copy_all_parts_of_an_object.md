条款 12：复制对象时勿忘其每一个成分
# 引子
一个设计良好的面向对象系统会将对象内部封装起来，只留得两个 function 用于拷贝对象：copy constructor 和 copy assignment operator。我们可以将这二者统称为 copy function。
在[[Item_5_Know_what_functions_C++_silentsly_writes_and_calls]]我们已获知，编译器会在必要时生成 copy function，且其行为是 copy 被拷贝对象的所有数据。

# 自定义 copy function 后编译器不会对其发出告警
然而当你声明自定义的 copy function 后，编译器不会对你的实现发出警告，即便其可能存在逻辑错误 bug，如下例：
~~~cpp
class Customer {
public:
    Customer() = default;
    Customer(const Customer& rhs) : name(rhs.name) { // 自定义 copy constructor
        std::cout << "call copy constructor" << std::endl;
    };
    Customer& operator=(const Customer& rhs) { // 自定义 copy assignment operator
        std::cout << "call copy assignment operator" << std::endl;
        name = rhs.name;
        return *this;
    };
private:
    std::string name;
};
~~~
上述代码并无问题。
此时，考虑该类后续新增了成员变量：
~~~cpp
class Data {};
class Customer {
private:
    std::string name;
    Data data; // 新增成员变量
public:
    Customer() = default;
    Customer(const Customer& rhs) : name(rhs.name) { // 此时，我们遗忘了加入新成员变量的拷贝
        std::cout << "call copy constructor" << std::endl;
    };
    Customer& operator=(const Customer& rhs) { // 此时，我们遗忘了加入新成员变量的拷贝
        std::cout << "call copy assignment operator" << std::endl;
        name = rhs.name;
        return *this;
    };
};
~~~
此时容易发生遗漏添加新增成员变量的拷贝，假若确实发生了，但编译器也并不会对此发出警告（毕竟是用户选择了自定义 copy function，编译器无法知晓什么是正确的拷贝逻辑，按用户实现为准也正常）。这也就导致容易埋下潜在的逻辑 bug，
因此，务必记住**当你为 class 添加了 data member，需确保同步更新了 copy function**。

# 继承场景下的 copy 问题

借用前文代码，继续考虑继承场景：
~~~cpp
class PriorityCustomer : public Customer {
    int priority;
public:
    PriorityCustomer() = default;
    PriorityCustomer(const PriorityCustomer& rhs) : priority(rhs.priority){
        std::cout << "call drived copy constructor" << std::endl;
    }
    PriorityCustomer& operator=(const PriorityCustomer& rhs) {
        std::cout << "call drived copy assignment operator" << std::endl;
        priority = rhs.priority;
        return *this;
    }
};
~~~

看似派生类的 copy function 已经完成了对派生类内所有成员变量的 copy，但实际上，派生类不仅具有自身的成员变量，还具有由基类继承而来的成员变量。
但上例中，copy function 仅对派生类自身的成员变量完成的 copy，而未对基类中的成员变量完成拷贝。
PriorityCustomer 的 copy constructor 并未传递实参给 Customer（即 PriorityCustomer 的 member initialization list 中未提及 Customer），因此 PriorityCustomer 会被 Customer 的无参构造函数初始化（default constructor）。
而 default constructor 只会对其成员变量进行默认初始化，并非 copy。

而在 PriorityCustomer copy assignment operator 中，略有不同只是，Customer 中的成员变量未执行 copy 操作，因此是保持不变，而非默认初始化。
不过这都与预期的 copy 操作不相符罢。

并且注意，即便如上例缺失了对父类中成员变量的 copy，编译器仍旧是不会报错的（毕竟是以用户为准），是编译通过的。

因此，我们需要使派生类的 copy function 调用父类的函数，以完成父类中成员变量的 copy：
~~~cpp
class PriorityCustomer : public Customer {
public:
    int priority;
    PriorityCustomer() = default;
    PriorityCustomer(const PriorityCustomer& rhs) : Customer(rhs), priority(rhs.priority){ // 正确的写法
        std::cout << "call drived copy constructor" << std::endl;
    }
    PriorityCustomer& operator=(const PriorityCustomer& rhs) {
        std::cout << "call drived copy assignment operator" << std::endl;
        Customer::operator=(rhs); // 需补上正确的写法
        priority = rhs.priority;
        return *this;
    }
};
~~~

# 不要互相调用 copy function
尽管 copy constructor 和 copy assignment operator 这两种 copy function 在功能上看似都很相似。甚至往往都有相近的函数体实现。但切记不要互相调用。

- 令 copy assignment operator 调用 copy constructor
    并无相关语法
- 令 copy constructor 调用 copy assignment operator
    constructor 用于初始化一个对象，此时对象尚未常见。而 copy assignment operator 用于对已完成初始化的对象进行赋值。
    因此在 constructor 中调用 copy assignment operator 是不合理的，尽管可以这么写出来代码。

如果确实存在大量相近代码，可以提炼出公共函数（往往是 private 的、且命名常为 init()）供二者调用，从而消除重复代码。

## 个人注：
在上一节[[Item_11_Handle_assignment_to_self_in_operator=#copy and swap]]中有讲述到 copy and swap 技巧从而实现异常安全的代码。
但我个人理解，本节里「令 copy assignment operator 调用 copy constructor」可能是指在 copy assignment operator 中通过调用 copy constructor 的方式直接实现 copy：
~~~cpp
class Test {
public:
    int num;
    Test() = default;
    Test(const Customer& rhs) : num(rhs.num) {};
    Test& operator=(const Test& rhs) {
        // num = rhs.num;
        *this(rhs) // 想这样，但并无这种语法
        return *this;
    };
};
~~~

而「令 copy constructor 调用 copy assignment operator」，我理解是指下述写法，实测可以这么写，但确实没什么意义：
~~~cpp
class Test {
public:
    int num;
    Test() = default;
    Test(const Customer& rhs) : num(rhs.num) {
    std::cout << "call copy constructor" << std::endl;
    *this = rhs; // 尽管确实可以这么写，但总有些奇怪，进入函数体时 data member 就已经完成默认初始化，这里再 copy
                 // 为什么不使用 member initializer list 一次初始化就完成 copy 操作呢
    // operator=(rhs); // 这么调用也可以
    };
    Test& operator=(const Test& rhs) {
        std::cout << "call copy assignment operator" << std::endl;
        num = rhs.num;
        return *this;
    };
};
~~~

以及，copy assignment operator 是对已有的对象进行操作，因此当我们使用/在 copy assignment operator 中时，应当是认为，要赋值到的对象是已存在的，因此如果 copy assignment operator 涉及到资源释放操作时，「令 copy constructor 调用 copy assignment operator」可能存在内存释放问题，例如使用[[Item_11_Handle_assignment_to_self_in_operator=]]中的类举例：
~~~cpp
class Widget {
public:
    Bitmap* pb;
    Widget(const Widget& rhs) {
        *this = rhs; // 此时 *this.pb 是随机值，指向随机位置
    }
	Widget& operator=(const Widget& rhs) {
    	// 写 copy assignment operator 时，我们认为是对已存在的对象进行操作，其是已完成初始化的，因此可能写下如下代码
    	// 然而，通过 constrctor 进入的 copy assignment operator，该对象可能并未完成初始化
    	// 也就可能导致了下面释放了随机位置内存
        Bitmap* pOrig = pb;
        pb = new Bitmap(*rhs.pb);
        delete pOrig; // 释放了随机位置的内存！
        return *this;
	}
};
~~~

除此之外，我还在网上发现一个危险场景[^1]，当 copy assignment operator 为值传递时，「令 copy constructor 调用 copy assignment operator」会导致无限递归：
~~~cpp
struct A
{
   A (const A &a) {
     *this = a; // Assignment to a not yet constructed object is not well
   }

   A &operator=(A a) { // 注意这里是 值传递，还会发生一次 copy 调用 copy constructor
   }
};

int main() {
    A a1, a2;
    a1 = a2; // 发生无限递归
    return 0;
}
~~~
因此，不建议「令 copy constructor 调用 copy assignment operator」。

# Thing to Remember
- Copying functions should be sure to copy all of an object's data members and all of its base class parts.
    copy function 应确保 copy 了对象的所有 data member 包含其父类的部分。
- Don't try to implement one of the copying functions in terms of the other. Instead, put common functionality in a third function that both call
    不要尝试以一个 copy function 中实现另一个 copy function。应将其公共部分抽象为第三个公共函数供二者调用。


[^1]: https://stackoverflow.com/questions/19616272/c-call-copy-assignment-in-copy-constructor

2025.02.17