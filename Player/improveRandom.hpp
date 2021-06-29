//
// Created by user on 2021/06/01.
//

#ifndef GEISTERLIBRARY_IMPROVERANDOM_HPP
#define GEISTERLIBRARY_IMPROVERANDOM_HPP
#include <string>
#include <algorithm>
#include "random.hpp"
#include "player.hpp"


class ImproveRandom: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
public:
    ImproveRandom(): mt(rd()){
    }

    virtual std::string decideRed(){
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        return pattern[serector(mt)];
    }

    virtual std::string decideHand(std::string_view res){
        game.setState(res);

        auto legalMoves = candidateHand();
        std::uniform_int_distribution<int> serector1(0, legalMoves.size() - 1);
        std::uniform_int_distribution<int> serector2(0, 100);
        auto action = legalMoves[serector1(mt) % legalMoves.size()];
        const std::array<Unit, 16>& units = game.allUnit();
        int distp[2],diste[2];
        diste[0]=diste[1]=distp[0]=distp[1]=100;
        int upt=serector2(mt);
        for(const Unit& u: units){
            if(u.color() == UnitColor::Blue){
                distp[0]=std::min(u.x()+u.y(),distp[0]);
                distp[1]=std::min(5-u.x()+u.y(),distp[1]);
                if(u.x() == 0 && u.y() == 0){
                    return Hand{u, Direction::West};
                }
                if(u.x() == 5 && u.y() == 0){
                    return Hand{u, Direction::East};
                }
                if(u.y() > 1 && upt < 20){
                    return Hand{u, Direction::North};
                }
            }else if(u.color() == UnitColor::Red){
                //diste[0]=std::min(u.x()+u.y(),diste[0]);
                //diste[1]=std::min(5-u.x()+u.y(),diste[1]);
            }
        }
        if(distp[0]<diste[0]){
            for(const Unit& u: units){
                if(u.color() == UnitColor::Blue){
                    if(distp[0] == u.x() + u.y()){
                        if(u.x() != 0)return Hand{u, Direction::West};
                        else return Hand{u, Direction::North};
                    }
                }
            }
        }
        if(distp[1]<diste[1]){
            for(const Unit& u: units) {
                if(u.color() == UnitColor::Blue) {
                    if (distp[1] == 5 - u.x() + u.y()) {
                        if (u.x() != 5)return Hand{u, Direction::East};
                        else return Hand{u, Direction::North};
                    }
                }
            }
        }
        int monthand=0;
        double mont=0.0;
        for(int i=0;i<legalMoves.size();i++){
            Geister game2=game;
            game2.move(legalMoves[i]);
            auto sim = Simulator(game2);
            double ras=sim.run(100);
            if(ras>mont)monthand=i;
           // if(i==0)printf("simran:%lf\n",ras);
        }
        //printf("montmax:%d\n",mont);
        action = legalMoves[monthand];
        return action;
    }

    virtual std::vector<Hand> candidateHand(){
        return game.getLegalMove1st();
    }
};
#endif //GEISTERLIBRARY_IMPROVERANDOM_HPP
