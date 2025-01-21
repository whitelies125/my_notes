条款 9：绝不在构造或析构函数中调用虚函数


## 引子 1
考虑以下示例，
Transaction 作为基类，BuyTransaction 和 SellTransaction 各自作为买和卖的子类。现希望在对象创建时记录日志，因此在使记录日志的函数 logTransaction() 为 virtual function，两个子类各自 override，并在父类构造函数中调用了 logTransaction()，希望以此实现该功能。
~~~cpp

class Transaction {
public:
    Transaction() {
        logTransaction(); // 实际上调用的总是父类 Transaction 的 logTransaction();
                         // 当然此处没有提供定义，因此链接时会报错
    }
    virtual void logTransaction() const = 0;
};

class BuyTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call BuyTransaction::logTransaction()" << std::endl;
    };
};

class SellTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call SellTransaction::logTransaction()" << std::endl;
    };
};

int main() {
    BuyTransaction b;
    return 0;
}
~~~
上述写法，会通过编译（但链接时失败）获得一个 warning，且报错信息为在 constructor 中调用了 pure virtual function，且注意到提示的被调用的函数是 Transaction::logTransaction()，即父类的 logTransaction 而非子类的。
同时，由于并未给 Transaction::logTransaction() 提供定义，因此在链接时会失败。
~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/test$ make
g++ -c --std=c++20 -Wall main.cpp -o main.o
main.cpp: In constructor ‘Transaction::Transaction()’:
main.cpp:6:23: warning: pure virtual ‘virtual void Transaction::logTransaction() const’ called from constructor
    6 |         logTransaction();
      |         ~~~~~~~~~~~~~~^~
g++ --std=c++20 -Wall main.o -o main.exe
/usr/bin/ld: main.o: in function `Transaction::Transaction()':
main.cpp:(.text._ZN11TransactionC2Ev[_ZN11TransactionC5Ev]+0x26): undefined reference to `Transaction::logTransaction() const'
collect2: error: ld returned 1 exit status
make: *** [Makefile:2: main.exe] Error 1
whitelies125@DESKTOP-H47PT8Q:~/code/test$
~~~

按我们朴素的预期而言，logTransaction() 是 virtual function，应该能够调用相应子类 override 的函数，因此构造 BuyTransaction 对象时，在父类 constructor 中调用 的应该是子类的 logTransaction()，从而记录不同的日志。
然而事实上，**尽管变量类型为子类，但在父类构造期间，virtual function 从不下降到子类，对象的行为如同父类一样**。
更通俗的说法，在父类构造期间，virtual function 实则并非 virtual。

## Why

- 因为在父类 constructor 先于子类 constructor 执行，所以**当父类 constructor 执行时，子类的 data member 尚未完成初始化**。
  如果父类构造时调用的 virtual function 下降到子类，而子类的成员函数几乎都会使用其 data member。
  在[[Item_4_Make_sure_that_objects_are_initialized_before_ther_are_used]]中我们已说明，使用数据前务必已初始化。
- 从原理上来讲，**在子类的父类构造/析构期间，该对象的类型是父类而不是子类**。
  此时，不仅仅 virtual function 会被编译器解析至父类，运行期类型信息（e.g.，dynamic_cast 和 typeid）也认为其是父类类型。

这么做是合理的：对象的子类部分尚未初始化，因此此时认为子类并不存在是最安全的做法。一个对象不能成为子类对象直到它子类 constructor 执行开始前。

destructor 也是同理。一旦子类 destructor 已执行，则该对象的子类 data member 应假定为未定义的值，C++ 也将其认为不再存在。当进入父类 destructor，则该对象变为父类类型对象。

## 引子 2
这类「constructor 或 destructor 中调用 virtual fucntion 导致不服预期」的错误并不总是如[[#引子 1]]中示例这般能够简单快速地发现的。

例如，父类可能有多个构造函数，每个构造函数都需要进行许多相同的操作，因此自然地将公共操作提取出 non-virtual 的 Init() 函数。
~~~cpp
class Transaction {
public:
    Transaction() {
        Init(); // constructor 中的 non-virtual function 中藏了对 virtual function 的调用
    }
    virtual void logTransaction() const = 0;
private:
    void Init() {
        logTransaction(); // 调用了 virtual，实际上调用的总是父类 Transaction 的版本
    }
};

class BuyTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call BuyTransaction::logTransaction()" << std::endl;
    };
};

class SellTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call SellTransaction::logTransaction()" << std::endl;
    };
};

int main() {
    BuyTransaction b;
    return 0;
}
~~~
然而我们失误地在 non-virtual 的 Init() 中调用了 virtual 的 logTransaction()。
此时进行编译，此前[[#引子 1]]中的 warning 与错误消失了，**编译器并没有给出 waring，也没有链接错误**。

只有在运行时才会因为在 constructor 中调用了 pure virtual function 而程序崩溃（见[[#个人注#抽象类的构造或析构函数中对纯虚函数的定义是未定义行为]]
~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/test$ make run
./main.exe
pure virtual method called
terminate called without an active exception
make: *** [Makefile:7: run] Aborted (core dumped)
whitelies125@DESKTOP-H47PT8Q:~/code/test$
~~~

更进一步，去掉 pure 并提供定义：
~~~cpp
class Transaction {
public:
    Transaction() {
        Init(); // constructor 中的 non-virtual function 中藏了对 virtual function 的调用
    }
    virtual void logTransaction() const; // 去掉 pure
private:
    void Init() {
        logTransaction(); // 调用了 virtual，实际上调用的总是父类 Transaction 的版本
    }
};
// 提供了父类中的定义
void Transaction::logTransaction() const {
    std::cout << "call Transaction::logTransaction()" << std::endl;
};


class BuyTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call BuyTransaction::logTransaction()" << std::endl;
    };
};

class SellTransaction : public Transaction {
public:
    virtual void logTransaction() const {
        std::cout << "call SellTransaction::logTransaction()" << std::endl;
    };
};

int main() {
    BuyTransaction b;
    return 0;
}
~~~

则无编译错误，无链接错误，可运行，运行结果如下。
~~~shell
whitelies125@DESKTOP-H47PT8Q:~/code/test$ make run
./main.exe
call Transaction::logTransaction()
whitelies125@DESKTOP-H47PT8Q:~/code/test$
~~~

个人注：这么写，实际上是符合C++语法的，个人理解本书是想说明这种写法不好，容易与人的朴素的直觉预期不符，但不是说语法不对，不符合C++规则。
构造或析构函数中调用 virtual function 详见[[#个人注#构造或析构函数中对虚函数的调用是其构造或析构函数所在的类中的版本]]。）

## 解决方法
如何实现不同子类构造时都打印正确的预期的日志呢。
简单的方式是，既然 virtual function 在 constructor 中无法向下调用，那么子类向父类构造函数向上传递信息即可：
~~~cpp
class Transaction {
public:
    // 子类需传递日志信息
    explicit Transaction(const std::string& logInfo) {
        logTransaction(logInfo); // 调用 Transaction::logTransaction()
    }
    // 改为 non-virtual 函数，避免迷惑用户
    void logTransaction(const std::string& logInfo) const {
        std::cout << logInfo << std::endl;
    };
};

class BuyTransaction : public Transaction {
public:
    // 将信息向上传递至父类
    BuyTransaction() : Transaction(CreateLogString()) {}
private:
    // 类的 static function，只能访问 static data memeber，避免了可能访问到未初始化的 non-static data member
    static std::string CreateLogString() {
        return "BuyTransaction";
    }
};

class SellTransaction : public Transaction {
public:
    SellTransaction() : Transaction(CreateLogString()) {}
private:
    static std::string CreateLogString() {
        return "SellTransaction";
    }
};

int main() {
    BuyTransaction b;
    return 0;
}
~~~

这里将生成传递信息的操作提取为了函数 CreateLogString()，这样做往往比较方便，可读性也更好。
需要额外注意的是，这里使用的是 static member function，因为 static member function 只能够访问类的 static data member 或 static member function（actually，这是因为 static 修饰的 data member 或 member function 并不绑定类），不能访问类的 non-static data member，从而避免了在此时子类尚未构造可能访问到未初始化的 data member 问题。
## 个人注

### 构造或析构函数中对虚函数的调用是其构造或析构函数所在的类中的版本

>[^1]
>When a virtual function is called directly or indirectly from a constructor or from a destructor (including during the construction or destruction of the class’s non-static data members, e.g. in a member initializer list), and the object to which the call applies is the object under construction or destruction, the function called is the final overrider in the constructor’s or destructor’s class and not one overriding it in a more-derived class. In other words, during construction or destruction, the more-derived classes do not exist.
>从 constructor 或 destructor 直接或间接调用 virtual function（包括在 class 的 non-static data member 构造或析构期间，如在 member initializer list 中），如果该调用作用的对象是正在构造或析构的对象，则被调用的 function 是 constructor 或 destructor 所在的类中的版本为最终 override 版本，而不是派生类中 override 的版本。换句话说，在构造或析构期间，派生类不存在。

### 抽象类的构造或析构函数中对纯虚函数的定义是未定义行为
>[^2]
>Making a virtual call to a pure virtual function from a constructor or the destructor of the abstract class is undefined behavior (regardless of whether it has a definition or not).
>从抽象类的构造函数或析构函数中对纯虚函数进行调用是未定义行为（无论其是否有定义）

[^1]: https://en.cppreference.com/w/cpp/language/virtual
[^2]: https://en.cppreference.com/w/cpp/language/abstract_class

2025.01.21