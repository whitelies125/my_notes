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

int main() {
    TimeKeeper *ptk = getTimerKeepr(1); // 获得指向子类的父类指针
    delete ptk; // 释放，行为未定义，实际往往会导致资源泄漏 // 仅打印 call ~TimeKeeper()

    Base * ptr = new Derived();
    delete ptr; // 释放，正确，子类与父类的 destructor 都打印
}
