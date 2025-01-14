#include <iostream>

class Bad {
public:
    // 由于自 C++11 起引入了 noexcept 关键字，且析构函数默认为 noexcept 所以需要手动声明为会抛出异常
    // 以及，不会抛出异常的函数（noexcept）是可以调用会抛出异常的函数，若不会抛出异常的函数抛出了异常，则调用 std::termate()
    ~Bad() noexcept(false) {
        std::cout << "~Bad()" << std::endl;
        throw 1; // 析构函数中抛出异常
    }
};

void example1() {
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
}

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

void example2() {
    func3();
}

int main() {
    example1();
    example2();
    return 0;
}
