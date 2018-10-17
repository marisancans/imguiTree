//
// Created by maris on 10/15/18.
//

#include "board.h"
#include "node.h"


void Board::initMoves() {
    _moves.push_back(&Board::moveUp);
    _moves.push_back(&Board::moveUpRight);
    _moves.push_back(&Board::moveRight);
    _moves.push_back(&Board::moveDownRight);
    _moves.push_back(&Board::moveDown);
    _moves.push_back(&Board::moveDownLeft);
    _moves.push_back(&Board::moveLeft);
    _moves.push_back(&Board::moveUpLeft);
}


std::vector<Movement> Board::getPossibleMoves(Node const* parent) {
    int a[MMC] = {};
    
    
    
    
    
    auto child = Node(parent);
    m.down[1] = 99;
//    pm = ^n->P1State.possMove
//
    return a;
}

