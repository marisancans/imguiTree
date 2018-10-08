#include "node.h"
#include "game.h"
#include "imgui/imgui.h"

void Game::getMoves(Node* n) {
    _graphLevel++;
    for(auto& fn : _moves)
        (this->*fn)(n);
}

Game::Game(Game::GameMode mode, Game::Turn turn) : _graphLevel(0) {
    initMoves();
    init();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    Node* n = new Node(int(_nodes.size()), genName(), ImVec2(0, 0), 0, 10, 3, 2, 2, inputs(), 0);
    _nodes.push_back(n);
    _turn = _turn == P1 ? P2 : P1;
    getMoves(n);
}

void Game::initMoves() {
    _moves.push_back(&Game::attack);
    _moves.push_back(&Game::defend);
    _moves.push_back(&Game::heal);
}

Node& Game::createChild(Node* parent) {
    Node* child = new Node(*parent, int(_nodes.size()), genName());
    child->addInput(parent);
    return *child;
}

void Game::attack(Node* parent){
    Node& child = createChild(parent);
    child.health -= parent->attack;
    if(child.isAlive())
        _nodes.push_back(&child);
}

void Game::defend(Node* parent){
    Node& child = createChild(parent);
    child.health -= parent->defense - parent->attack;
    if(child.isAlive())
        _nodes.push_back(&child);
}

void Game::heal(Node* parent){
    Node& child = createChild(parent);
    child.health += child.heal;
    if(child.isAlive())
        _nodes.push_back(&child);
}





