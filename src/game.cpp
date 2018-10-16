#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



void Game::genLayers(int count) {
    int y = 0;
    int x = 0;
    for (auto &l : _nodes)
        for (auto &n : l) {
            auto currState = y % 2 == 0 ? n->P1State : n->P2State;
            _board->getPossibleMoves(currState);
        }
//    _board->getPossibleMoves();
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), _turn(turn), gameSettings(gameSettings)
{
    initMoves();
    init();
}

void Game::init() {
    _board = new Board(gameSettings->maxBoardX, gameSettings->maxBoardY,
                       gameSettings->P1MovRange, gameSettings->P2MovRange);

    // Initial layer creation
    auto n = Node(getNodeID(), Node::ROOT);
    NODE_VEC v;
    v.push_back(&n);
    _nodes.push_back(v);
    genLayers(gameSettings->maxLayer);
}

void Game::initMoves() {
    _moves.push_back(&Game::attack);
    _moves.push_back(&Game::defend);
    _moves.push_back(&Game::heal);
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


