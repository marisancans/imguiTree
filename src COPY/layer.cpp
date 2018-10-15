//
// Created by maris on 10/9/18.
//

#include "layer.h"
#include "node.h"
#include <algorithm>

bool Layer::linkDuplicate(Node *child) {
//    std::any_of(_nodes.begin(), _nodes.end(), [](Node* n){return });
}

// Spaghetti
int Layer::getLowestNodeHP(){
    int min = _currentTurn == Game::Turn::P1 ?
              _nodes.back()->P1Stats.health :  _nodes.back()->P2Stats.health;

    auto cmp = [this](Node* n1, Node* n2){
        return this->_currentTurn == Game::Turn::P1 ?
            n1->P1Stats.health < n2->P1Stats.health : n2->P2Stats.health < n1->P2Stats.health; };

    Node* x = *std::min_element(_nodes.begin(), _nodes.end(), cmp);

    return _currentTurn == Game::Turn::P1 ? x->P1Stats.health : x->P2Stats.health;
}

