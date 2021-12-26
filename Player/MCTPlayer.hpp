#include <string>
#include <stack>
#include <chrono>
#include "random.hpp"
#include "player.hpp"
#include "simulator.hpp"

class MCTPlayer: public Player{
    cpprefjp::random_device rd;
    std::mt19937 mt;
public:
    MCTPlayer(): mt(rd()){
    }

    virtual std::string decideRed(){
        cpprefjp::random_device rd;
        std::mt19937 mt(rd());

        std::uniform_int_distribution<int> serector(0, pattern.size() - 1);
        return pattern[serector(mt)];
    }

    virtual std::string decideHand(std::string_view res){
        int t = 0;
        int fircount = 3;
        int index_max = 0;

        game.setState(res);

        auto legalMoves = candidateHand();
        for( auto move : legalMoves) {
            Unit u = move.unit;
            if(u.isBlue() && (u.x() == 0 && u.y() == 0) ) { return Hand{u, 'W'}; }
            if(u.isBlue() && (u.x() == 5 && u.y() == 0) ) { return Hand{u, 'E'}; }
        }

        std::vector<Simulator> simulators;
        for( auto m : legalMoves ) {
            Simulator s{game};
            s.root.move(m);
            simulators.push_back(s);
        }
        std::vector<int> tries(legalMoves.size(), 0);
        std::vector<double> rewards(legalMoves.size(), 0.0);
        for (int i = 0; i < legalMoves.size(); i++) {
            rewards[i] += simulators[i].run(fircount*10);
            tries[i] += fircount*10;
            t += fircount*10;
        }
        long long int count = 0;
        auto start = std::chrono::system_clock::now();
        while (count <= 1000 * legalMoves.size()) {
            for (int i = 1; i < legalMoves.size(); i++) {

                if ((rewards[index_max] + 1) / (double)(2 * tries[index_max]) + sqrtl(2 * log(t / tries[index_max]))
                    < (rewards[i] + 1) / (double)(2 * tries[i]) + sqrtl(2 * log(t / tries[i]))) {
                    index_max = i;
                }
            }
            //for (int i = 0; i < 1; i++) {
                rewards[index_max] += simulators[index_max].run(fircount);
                tries[index_max] += fircount;
                t += fircount;
            //}
            count++;
            if(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start).count()>20)break;
        }

        return legalMoves[index_max];
    }

    virtual std::vector<Hand> candidateHand(){
        return game.getLegalMove1st();
    }
    virtual double montetree(Geister g, int n) {
        int upper = 50;
        int fircount = 10;
        int index_max = 0;
        int t = 0;
        double UCB1;
        std::stack<Geister> S;
        auto legalMoves = g.getLegalMove1st();
        std::vector<double> rewards(legalMoves.size(), 0.0);
        std::vector<int> tries(legalMoves.size(), 0);
        std::vector<Simulator> simulators;
        for (auto m : legalMoves) {
            Simulator s{ game };
            s.root.move(m);
            simulators.push_back(s);
        }
               
        for (int i = 0; i < legalMoves.size(); i++) {
            rewards[i] += simulators[i].run(fircount);
            tries[i] += fircount;
            t += fircount;
        }
        int count = 0;
        while (count >= 2000 * legalMoves.size()) {
            for (int i = 1; i < legalMoves.size(); i++) {

                if ((rewards[index_max] + 1) / (double)(2 * tries[index_max]) + sqrtl(2 * log(t / tries[index_max]))
                    < (rewards[i] + 1) / (double)(2 * tries[i]) + sqrtl(2 * log(t / tries[i]))) {
                    index_max = i;
                }
            }
            for (int i = 0; i < fircount; i++) {
                rewards[i] += simulators[i].run(fircount);
                tries[i] += fircount;
                t += fircount;
            }
            count++;
        }
        S.push(g);
        

        /*while (!S.empty()) {
            Geister s = S.top();
            S.pop();
            if (s.isEnd()) {
                if(n >= upper)
            }
            else {
                std::vector<Hand> hand = s.getLegalMove1st();
                for (int i = 1; i < hand.size(); i++) {
                    
                }
                if (n != 0) = (1 + w) / (2 * n) + 2 * pow(log(double(t / n)), 0.5);
                else UCB1
            }

        }*/
    }
};