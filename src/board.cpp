//
// Created by maris on 10/15/18.
//

#include "board.h"
#include "node.h"


POS_VEC Board::getPossibleMoves(const Position* parentPos) {
    moveMatrix mov;

    for(int x = 0; x < _xMax; ++x){
        for(int y = 0; y < _yMax; ++y){
            mov[UP][0] = 1;


        }
    }

    
    POS_VEC pv;
    Position p{5, 5};
    pv.push_back(p);
    
    
    
//    auto child = Node(parent);
//    m.down[1] = 99;
//    pm = ^n->P1State.possMove
//
    return pv;
}

