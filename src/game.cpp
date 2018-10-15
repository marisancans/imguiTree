#include "game.h"
#include "node.h"

#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>



void Game::genLayers(int count) {
    auto n = Node(getNodeID(), Node::ROOT);


}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), _turn(turn), gameSettings(gameSettings)
{
    _board = Board(gameSettings->maxBoardX, gameSettings->maxBoardY);
    initMoves();
    init();
}

void Game::init() {
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


