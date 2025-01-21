#include <iostream>

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
