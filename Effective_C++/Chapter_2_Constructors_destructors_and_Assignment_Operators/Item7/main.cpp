#include <iostream>

class TimeKeeper {
public:
    TimeKeeper () {};
    ~TimeKeeper() {
        std::cout << "call ~TimeKeeper()" << std::endl;
    }
};

class AtomicClock : public TimeKeeper {
public:
    ~AtomicClock() {
        std::cout << "call ~AtomicClock()" << std::endl;
    }
};
class WaterClock : public TimeKeeper {};
class WristWatch : public TimeKeeper {};

class Base {
public:
    virtual ~Base() {
        std::cout << "call ~Base()" << std::endl;
    }
};

class Derived : public Base {
public:
    virtual ~Derived() {
        std::cout << "call ~Derived()" << std::endl;
    }
};

// 工厂方法
TimeKeeper * getTimerKeepr(int type){
    switch(type) {
        case 1: return new AtomicClock();
        case 2: return new WaterClock();
        case 3: return new WristWatch();
        default: return nullptr;
    }
}

class AWOV {
public:
    virtual void func() = 0;
    virtual ~AWOV() = 0;
};
// 可为纯虚函数提供定义
void AWOV::func() {
    std::cout << "call AWOV func()" << std::endl;
}
// 当纯虚函数为析构函数时，则必须为其提供定义
AWOV::~AWOV() {
    std::cout << "call ~AWOV()" << std::endl;
}

class SonAWOV : public AWOV {
public:
    virtual void func() override {
        std::cout << "call SonAWOV func()" << std::endl;
        AWOV::func(); // 纯虚函数也可有定义，可以这样在子类中 base::function 的方式调用
                      // 但注意，在构造、析构函数中调用纯虚函数是未定义行为
    };
    ~SonAWOV() {
        AWOV::func(); // 未定义行为
        std::cout << "call ~SonAWOV()" << std::endl;
    }
};

int main() {
    TimeKeeper *ptk = getTimerKeepr(1); // 获得指向子类的父类指针
    delete ptk; // 释放，行为未定义，实际往往会导致资源泄漏 // 仅打印 call ~TimeKeeper()

    Base * ptr = new Derived();
    delete ptr; // 释放，正确，子类与父类的 destructor 都打印

    AWOV *sonAwov = new SonAWOV();
    sonAwov->func();
    delete sonAwov;
}
