//
// Created by maris on 10/15/18.
//

#include "board.h"
#include "node.h"


POS_VEC Board::getPossibleMoves(const Position& p) {

    POS_VEC pv;
    moveMatrix mov{};

    auto canRight = [&](int i){ return p.x + i > 0 && p.x + i <= _xMax; };
    auto canLeft = [&](int i){ return p.x - i >= 0 && p.x - i < _xMax; };
    auto canUp = [&](int i){ return p.y - i >= 0 && p.y - i < _yMax; };
    auto canDown = [&](int i){ return p.y + i > 0 && p.y + i <= _yMax; };

    // Up
    for(int i = 1; i < MRR + 1; ++i){
        if(canUp(i)) {
            mov[UP][i] = 1;
            pv.push_back(Position{p.x, p.y - i});
        }

        if(canRight(i) && canUp(i)) {
            mov[UP_RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y - i});
        }

        if(canRight(i)) {
            mov[RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y});
        }

        if(canRight(i) && canDown(i)) {
            mov[DOWN_RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y + i});
        }

        if(canDown(i)) {
            mov[DOWN][i] = 1;
            pv.push_back(Position{p.x, p.y + i});
        }

        if(canLeft(i) && canDown(i)) {
            mov[DOWN_LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y + i});
        }

        if(canLeft(i)) {
            mov[LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y});
        }

        if(canLeft(i) && canUp(i)) {
            mov[UP_LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y + i});
        }

    }

    return pv;
}

