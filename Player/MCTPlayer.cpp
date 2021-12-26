#include <string>
#include <iostream>
#include <memory>
#include "MTCPlayer.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
MCTPlayer* createPlayer(){
    return new MCTPlayer();
}
}
