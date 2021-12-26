#include <string>
#include <iostream>
#include <memory>
#include "MCTPlayer.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
MCTPlayer* createPlayer(){
    return new RandomPlayer();
}
}
