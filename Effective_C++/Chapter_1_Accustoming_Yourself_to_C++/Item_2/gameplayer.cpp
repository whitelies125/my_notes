#include <iostream>

#include "gameplayer.h"

const int GamePlayer::NumTurns;
// const int GamePlayer::NumTurns = 1; //Static data member 'NumTurns' already has an initializer [static_data_member_reinitialization]

void GamePlayer::print() {
    std::cout << NumTurns << std::endl;
}
