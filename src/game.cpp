#include "game.h"
#include "node.h"
#include "layer.h"
#include "imgui/imgui.h"

#include <iostream>
#include <algorithm>

void Game::getNextLayer() {
    auto l = new Layer(_layers.back()->getLevel() + 1);
    auto prevNodes = _layers.back()->getNodes();

    for(auto & parent : prevNodes) {
        for (auto &fn : _moves) {
            Node &child = (this->*fn)(parent);
//            auto cmpLambda = [parent](auto child){ return &child != parent; };
//            bool unique = std::any_of(prevNodes.back(), prevNodes.end(), cmpLambda());
            if (child.isAlive() ) {


                l->addNode(&child);
            } else {
                child.setStatus(Node::END);
                return;
            }
        }
    }

    _layers.push_back(l);
    getNextLayer();

    //printf("%i\n", n->health);
    //for(auto const& c : n->childrenNodes)
    //    if(c->status != Node::END)
    //        getNextLayer(c);
}

Game::Game(Game::GameMode mode, Game::Turn turn) : _nodeCount(0){
    initMoves();
    init();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    auto l = new Layer(0);
    auto n = new Node(getNodeID(), genName(), ImVec2(0, 0), 0, 10, 3, 2, 2, inputs(), 0, Node::ROOT);
    l->addNode(n);
    _layers.push_back(l);
    _turn = _turn == P1 ? P2 : P1;
    getNextLayer();
}

void Game::initMoves() {
    _moves.push_back(&Game::attack);
    _moves.push_back(&Game::defend);
    _moves.push_back(&Game::heal);
}

Node& Game::createChild(Node* parent) {
    Node* child = new Node(getNodeID(), *parent, genName());
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





