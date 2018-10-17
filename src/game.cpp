#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



void Game::genLayer() {
        for(auto& parent : _nodes.back()) { // Last vector (Parent nodes)
            _board->getPossibleMoves(parent);
        }
//    _board->getPossibleMoves();
    swapTurn();
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), _turn(turn), gameSettings(gameSettings)
{
    initMoves();
    init();
    genLayer();
}

void Game::init() {
    _board = new Board(gameSettings->maxBoardX, gameSettings->maxBoardY,
                       gameSettings->P1MovRange, gameSettings->P2MovRange);

    // Initial layer creation
    auto n = Node(getNodeID(), Node::ROOT);
    NODE_VEC v;
    v.push_back(&n);
    _nodes.push_back(v);
    swapTurn();
}



Node *Game::createChild(Node *parent) {
    Node* child = new Node(getNodeID(), *parent);
    return child;
}

Node* Game::attack(Node* parent){
    auto child = createChild(parent);
    return child;
}

Node* Game::defend(Node* parent){
    auto child = createChild(parent);
    return child;
}

Node* Game::heal(Node* parent){
    auto child = createChild(parent);
    return child;
}


