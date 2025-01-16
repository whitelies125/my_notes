条款 8：阻止异常离开析构函数

## 引子，二次抛出异常导致程序终止
C++ 没有禁止 destructor 发出异常，但并不鼓励这种做法。
考虑一下示例：
~~~cpp
class Widge {
public:
    ~Widge() {} // 假定该析构函数可能抛出异常
};

void func() {
    std::vector<Widge> vec(10);
} // func() 调用结束时，v 会析构。
~~~
在这里，函数 func() 中，创建了大小为 10 的 Widge 的 vector 容器 vec，函数调用结束时作为局部变量的 vec 会析构。
假若 vec 容器析构时中途某个元素的 destructor 抛出异常，但 vec 作为容器，其析构时容器中的所有元素都应该被析构，因此 vec 应继续调用后续元素的 destructor，否则可能导致资源泄漏。
而继续调用后续元素的 destructor 可能再次出现了异常，此时会导致程序崩溃或未定义行为。

（个人注，按照 cppreference 上 throw 页面[^1]所诉，未处理异常时再次抛出异常的处理是调用 std::terminate()）。

>[^1]
>If a program attempts to rethrow an exception when no exception is presently being handled, std::terminate will be invoked. Otherwise, the exception is reactivated with the existing exception object (no new exception object is created), and the exception is no longer considered to be caught.
>如果程序试图在当前没有正在处理的异常时重新抛出异常，将调用 `std::terminate`。否则，异常会被重新激活，使用现有的异常对象（不会创建新的异常对象），并且该异常将不再被视为已捕获。

（个人注：同时，按 cppreference 上 std::terminate 页面所述）
>std::terminate() is called by the C++ runtime when the program cannot continue for any of the following reasons:
>C++ 运行时当程序因以下任一原因而无法继续运行时则调用：std::terminate()
>...
>1) A function directly invoked by the exception handling mechanism while handling an exception that has not yet been caught exits via an exception (e.g. a destructor of some local object, or a copy constructor constructing a catch-clause parameter).
>一个函数直接被异常处理机制调用，当其正常捕获一个异常但尚未处理
>3) The constructor or the destructor of a static or thread-local(since C++11) object throws an exception.
>static（或 thread-local 自 C++11起）对象的构造或析构函数抛出异常。
>5) ...

又如：
~~~cpp
class Bad {
public:
    // 由于自 C++11 起引入了 noexcept 关键字，且析构函数默认为 noexcept 所以需要手动声明为会抛出异常
    // 以及，不会抛出异常的函数（noexcept）是可以调用会抛出异常的函数，若不会抛出异常的函数抛出了异常，则调用 std::termate()
    ~Bad() noexcept(false) {
        std::cout << "~Bad()" << std::endl;
        throw 1; // 析构函数中抛出异常
    }
};

int main() {
    try{
        Bad bad; // Stack Unwinding 时会析构作用域内变量，bad 的析构函数再次抛出异常
        std::cout << "throw 0" << std::endl;
        throw 0; // 抛出异常，开始 Stack Unwinding
    }
    catch(...) {
        // 上述代码 bad 析构时二次抛出异常时，调用 std::termate()，程序终止，因此此处不会打印
        std::cout << "never print 1" << std::endl;
    }
    // 也不会执行到这里，此处不会打印
    std::cout << "never print 2" << std::endl;
    return 0;
}
~~~


## 优化方式
有时难以避免在 destructor 中进行可能抛出异常的操作。
考虑以下示例：
~~~cpp
class DBConnection {
public:
    static DBConnection create();
    void close(); // 关闭链接，可能失败，若失败则抛出异常
};

// DBConnection 的管理类
class DBConn {
public:
    DBConn(DBConnection db_): db(db_) {}
    ~DBConn() {
        db.close();
    }
private:
    DBConnection db;
};

void func() {
    //...
    DBConn dbc(DBConnection::create());
    //...
}
// dbc 在区块结束时析构，destructor 中调用了 close()，可能抛出异常

int main() {
    func();
}
~~~

DBConn 作为 DBConnection 的管理类，在析构时调用 close() 函数，起到自动关闭链接的作用，免去用户需要手动 close 的操作，减少用户遗忘关闭链接进而导致问题的可能。

### 在析构函数中处理掉异常
下述两种方式都是使得异常不跑出析构函数，选用哪种需视具体情况而定。

#### 终止程序
当发生该异常若不处理则无法继续运行后续程序时，我们可以主动记录异常过程并终止程序，防止抛出异常进而导致后续的不确定行为。
~~~cpp
    ~DBConn() {
        try {
            db.close();
        } catch (...) {
            // do something 如记录日志
            std::abort();
        }
    }
~~~
#### 不终止程序

当异常不会对程序后续结果产生不良影响的情况下，程序的持续运行更显重要时，我们也可以选择将异常接住并记录日志当并不终止程序，然后让程序继续进行。
~~~cpp
    ~DBConn() {
        try {
            db.close();
        } catch (...) {
            // do something 如记录日志
        }
    }
~~~


#### 重新设计接口让用户可以主动处理该异常
以上两种做法都属于设计者自行决定了如何处理异常，而使用者则无法自行进行处理。
因此，一个较好的选择是，重新设计接口，使得用户有机会对可能出现的问题作出反应。
即单独提供一个可能发生异常的操作的函数供用户调用并自己决定如何处理可能的异常，同时在析构函数中也对用户遗漏调用该函数的场景进行保护：
~~~cpp
class DBConn {
public:
    DBConn(DBConnection db_): db(db_) {}
    void close() { // 提供给用户调用，用户主动关闭链接并自行处理可能的异常
        db.close();
        closed = true;
    }
    ~DBConn() {
        if (!colsed) { // 放置用户遗漏调用的保护处理
            try {
                db.close();
            } catch (...) {
                // do something
            }
        }
    }
private:
    DBConnection db;
    bool closed;
};
~~~

（个人注：个人理解，相当于是默认提供了一个实现，用户可以主动调用 close() 来复写。）
## 个人注
### Stask Unwinding 栈展开、栈回溯

> [^1]
> ...
> Once the exception object is constructed, the control flow works backwards (up the call stack) until it reaches the start of a try block, at which point the parameters of all associated handlers are compared, in order of appearance, with the type of the exception object to find a match. If no match is found, the control flow continues to unwind the stack until the next try block, and so on. If a match is found, the control flow jumps to the matching handler.
> 一旦异常对象被构造完成，控制流会沿调用栈向上回溯，直到到达某个 try 块的起始位置。这时，程序会按顺序将所有关联 handler 的参数与异常对象的类型进行比较，以寻找匹配项。如果没有找到匹配项，控制流会继续沿调用栈向上回溯，直到找到下一个 try 块，依此类推。如果找到匹配项，控制流会跳转到相应的 handler。
> As the control flow moves up the call stack, destructors are invoked for all objects with automatic storage duration that are constructed, but not yet destroyed, since the corresponding try block was entered, in reverse order of completion of their constructors. If an exception is thrown from a destructor of a local variable or of a temporary used in a return statement, the destructor for the object returned from the function is also invoked.
> 当控制流沿调用栈向上回溯时，会调用自进入相应 try 块之后的、所有具有自动存储期限的、且未销毁的对象的析构函数，析构顺序与它们构造顺序相反。如果在局部变量的析构函数或返回语句中使用的临时对象的析构函数中抛出了异常，返回的对象的析构函数也会被调用。
> ...
> This process is called stack unwinding.
> 此过程称为**stack unwinding（栈展开、栈回溯）**。

简而言之，如下示例，在 fun1() 中 抛出异常，向上寻找类型匹配的异常处理 handler，并按作用域内构造顺序的反序，对作用域内的具有自动存储期限的、且未销毁的对象进行析构。

~~~cpp
void func1() {
    std::cout << "func1 starts" << std::endl;
    int i; // step 2：按作用域内构造顺序的反序，析构所有具有自动存储期限且未销毁的对象
    std::string str("func1"); // step 1：按作用域内构造顺序的反序，析构所有具有自动存储期限且未销毁的对象
    throw 100; // step 0：抛出 int 异常，程序开始在栈中搜索对应的异常处理器，即开始栈展开
    std::cout << "func1 ends" << std::endl; //这行代码不会被执行
}

void func2() {
    std::cout << "func2 starts" << std::endl;
    std::string str("func2"); // step 4：按作用域内构造顺序的反序，析构所有具有自动存储期限且未销毁的对象
    try { // step 3：func1 没有搜索到对应的异常处理，因此返回到 func2 并找到了 try，但并无匹配的 handler
        func1();
    } catch (std::string s) {
        std::cout << "func2 catch" << std::endl; // 不会打印
    }
    std::cout << "func2 ends" << std::endl; // 不会打印
}

void func3(){
    std::cout << "func3 starts" << std::endl;
    try{ //step 5：func2 也没有找到异常处理，最后返回了 func 3并找到了异常处理
        func2();
    } catch (int i) {
    // step 6: 跳转到类型匹配的 handle 中进行处理
        std::cout << "func3 catch" << i << std::endl;
    }
    std::cout << "func3 ends" << std::endl; // 会打印
}

int main(){
    func3();
    return 0;
}
~~~

这也是[[#引子，二次抛出异常导致程序终止]]中第二个代码示例，为什么会二次抛出异常。因为抛出异常时，stack unwinding 会析构此前已构造所有具有自动存储期限且未销毁的对象，也就导致了若析构的对象的 destructor 抛出异常，则程序终止。

## Thing to Remember

>Thing to Remember
>- Destructors should never emit exceptions. If functions called in a destructor may throw, the destructor should catch any exceptions, then swallow them or terminate the program.
>析构函数应永不抛出异常。如果析构函数中调用的函数可能抛出异常，则析构函数应捕获所有异常，然后处理掉它或终止程序。
>- If class clients need to be able to react to exceptions thrown during an operation, the class should provide a regular (i.e., non-destructor) function that performs the operation.
>如果类的用户需要对一个操作抛出的异常做出反应，则该类应当提供一个执行该操作的普通（如析构函数意外的）函数。

[^1]: https://en.cppreference.com/w/cpp/language/throw
[^2]: https://zh.cppreference.com/w/cpp/error/terminate

2025.01.16