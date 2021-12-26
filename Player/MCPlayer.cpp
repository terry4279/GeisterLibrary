#include <string>
#include <iostream>
#include <memory>
#include "MCPlayer.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
MCPlayer* createPlayer(){
    return new MCPlayer();
}
}
