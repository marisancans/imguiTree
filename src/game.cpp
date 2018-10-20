#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>



void Game::genLayer() {
    NODE_VEC newLayer;
    std::unordered_set<Position> setOfPositions;

    for(auto parent : _nodes.back()) { // Last vector (Parent nodes)
        auto parentPos = _turn == P1 ? &parent->P1Pos : &parent->P2Pos;
        POS_VEC possPositions = _board->getPossibleMoves(*parentPos);

        int nth = 0;
        for(auto& pos : possPositions){
            auto res = setOfPositions.insert(pos);

            if (res.second) {
                auto child = new Node(nth++, parent);
                if (_turn == P1)
                    child->P1Pos = pos;
                else
                    child->P2Pos = pos;
                newLayer.push_back(child);
            }
        }

    }

    _nodes.push_back(newLayer);

    swapTurn();
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _turn(turn), gameSettings(gameSettings)
{
    init();

    for(int i = 0; i < gameSettings->maxLayer; ++i){
        genLayer();
    }
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
