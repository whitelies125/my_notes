#include <iostream>

class Data {
public:
    int num;
};
class Customer {
public:
    std::string name;
    Data data;
    Customer() = default;
    Customer(const Customer& rhs) : name(rhs.name) { // 自定义 copy constructor
        std::cout << "call copy constructor" << std::endl;
    };
    Customer& operator=(const Customer& rhs) { // 自定义 copy assignment operator
        std::cout << "call copy assignment operator" << std::endl;
        name = rhs.name;
        return *this;
    };
};

class PriorityCustomer : public Customer {
public:
    int priority;
    PriorityCustomer() = default;
    PriorityCustomer(const PriorityCustomer& rhs) : priority(rhs.priority){
    // PriorityCustomer(const PriorityCustomer& rhs) : Customer(rhs), priority(rhs.priority){ // 正确的写法
        std::cout << "call drived copy constructor" << std::endl;
    }
    PriorityCustomer& operator=(const PriorityCustomer& rhs) {
        std::cout << "call drived copy assignment operator" << std::endl;
        // Customer::operator=(rhs); // 需补上正确的写法
        priority = rhs.priority;
        return *this;
    }
};

void example_1() {
    Customer c1;
    c1.name = "C";
    c1.data.num = 1;
    Customer c2(c1); // 只 copy 了 name
    std::cout << c1.name << ", " << c1.data.num << std::endl;
    std::cout << c2.name << ", " << c2.data.num << std::endl;
}

void example_2() {
    PriorityCustomer p1;
    p1.name = "P";
    p1.data.num = 1;
    p1.priority = 2;
    PriorityCustomer p2(p1); // 只 copy 了 priority
    std::cout << p1.name << ", " << p1.data.num << ", " << p1.priority << std::endl;
    std::cout << p2.name << ", " << p2.data.num << ", " << p2.priority << std::endl;
}

class Test {
public:
    int num;
    Test() = default;
    Test(const Test& rhs) {
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

void example_3() {
    Test t1;
    t1.num = 1;
    Test t2(t1);
    std::cout << t1.num << std::endl;
    std::cout << t2.num << std::endl;
}

int main() {
    example_1();
    example_2();
    example_3();
    return 0;
}
