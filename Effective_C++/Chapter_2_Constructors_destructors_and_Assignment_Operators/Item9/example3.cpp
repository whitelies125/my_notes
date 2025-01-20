#include <iostream>

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
