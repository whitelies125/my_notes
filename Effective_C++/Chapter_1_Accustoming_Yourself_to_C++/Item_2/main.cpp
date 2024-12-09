#include <iostream>

#include "gameplayer.h"
int main() {
    std::cout << Turns << std::endl; // 尽管 #define Turns 放在 private 中，也可直接访问，因为 #define 与作用域无关，无法提供封装性
    // std::cout << GamePlayer::NumTurns << std::endl; // 'NumTurns' is a private member of 'GamePlayer' [access]
    GamePlayer obj;
    // std::cout << obj.NumTurns << std::endl; // 'NumTurns' is a private member of 'GamePlayer' [access]
    obj.print();
}
