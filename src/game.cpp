#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



void Game::genLayer() {
    for(auto parent : _nodes.back()) { // Last vector (Parent nodes)
        auto pos = _turn == P1 ? &parent->P1Pos : &parent->P1Pos;
        _board->getPossibleMoves(*pos);
    }

    swapTurn();
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), _turn(turn), gameSettings(gameSettings)
{
    init();
    genLayer();
}

void Game::init() {
    _board = new Board(gameSettings->maxBoardX, gameSettings->maxBoardY,
                       gameSettings->P1MovRange, gameSettings->P2MovRange);

    // Initial layer creation
    auto n = new Node(getNodeID(), Node::ROOT, gameSettings->P1StartPos, gameSettings->P2StartPos);
    NODE_VEC v;
    v.push_back(n);
    _nodes.push_back(v);
    swapTurn();
}
