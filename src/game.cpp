#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



void Game::genLayer() {
    for(auto parent : _nodes.back()) { // Last vector (Parent nodes)
        auto pos = _turn == P1 ? &parent->P1Pos : &parent->P1Pos;
        POS_VEC possPositions = _board->getPossibleMoves(*pos);
        NODE_VEC newLayer;

        for(int i = 0; i < possPositions.size(); ++i){
            auto child = new Node(i, parent);
            newLayer.push_back(child);
        }

        _nodes.push_back(newLayer);
    }

    swapTurn();
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _turn(turn), gameSettings(gameSettings)
{
    init();
    genLayer();
}

void Game::init() {
    _board = new Board(gameSettings->maxBoardX, gameSettings->maxBoardY,
                       gameSettings->P1MovRange, gameSettings->P2MovRange);

    // Initial layer creation
    auto n = new Node(0, Node::ROOT, gameSettings->P1StartPos, gameSettings->P2StartPos);
    NODE_VEC v;
    v.push_back(n);
    _nodes.push_back(v);
    swapTurn();
}
