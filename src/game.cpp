#include "node.h"
#include "game.h"
#include "imgui/imgui.h"

#include <iostream>

void Game::getLevel(Node *n) {
    _graphLevel++;
    static int iii = 0;




    for(auto& fn : _moves) {
        Node& child = (this->*fn)(n);
        if (child.isAlive()) {
            _nodes.push_back(&child);
        } else {
            n->status = Node::END;
            return;
        }
    }
    iii++;
    //if(iii == 10) return;
    printf("%i\n", n->health);
    for(auto const& c : n->childrenNodes)
        if(c->status != Node::END)
            getLevel(c);
}

Game::Game(Game::GameMode mode, Game::Turn turn) : _graphLevel(0) {
    initMoves();
    init();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    Node* n = new Node(int(_nodes.size()), genName(), ImVec2(0, 0), 0, 10, 3, 2, 2, inputs(), 0, Node::ROOT);
    _nodes.push_back(n);
    _turn = _turn == P1 ? P2 : P1;
    getLevel(n);
}

void Game::initMoves() {
    _moves.push_back(&Game::attack);
    _moves.push_back(&Game::defend);
    _moves.push_back(&Game::heal);
}

Node& Game::createChild(Node* parent) {
    Node* child = new Node(*parent, int(_nodes.size()), genName());
    return *child;
}

Node& Game::attack(Node* parent){
    Node& child = createChild(parent);
    child.health -= parent->attack;
    return child;
}

Node& Game::defend(Node* parent){
    Node& child = createChild(parent);
    child.health -= parent->defense - parent->attack;
    return child;
}

Node& Game::heal(Node* parent){
    Node& child = createChild(parent);
    child.health += child.heal;
    return child;
}





