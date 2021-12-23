//
// Created by user on 2021/06/01.
//
#include <string>
#include <iostream>
#include <memory>
#include "improveRandom.hpp"

extern "C"{
#ifdef _WIN32
__declspec(dllexport)
#endif
RandomPlayer* createPlayer(){
    return new ImproveRandom();
}
}
