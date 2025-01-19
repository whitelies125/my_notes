#include <iostream>

class Transaction {
public:
    Transaction() {
        logTransaction(); // 实际上调用的总是父类 Transaction 的 logTransaction();
    }
    virtual void logTransaction() const = 0;
};

/*
class Transaction {
public:
    Transaction() {
        Init(); // constructor 中的 non-virtual function 中藏了对 virtual function 的调用
    }
    virtual void logTransaction() const = 0;
private:
    void Init() const {
        std::cout << "call Transaction::Init()" << std::endl;
        logTransaction(); // 调用了 virtual，实际上调用的总是父类 Transaction 的版本
    }
};
*/

/*
void Transaction::logTransaction() const {
    std::cout << "call Transaction::logTransaction()" << std::endl;
};
*/

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



void example1() {
    BuyTransaction b;
}

int main() {
    example1();
    return 0;
}
