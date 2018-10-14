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
    int min = 0;
    auto cmp = [this](Node* n1, Node* n2){
        return this->_currentTurn == Game::Turn::P1 ?
            n1->P1Stats.health < n2->P1Stats.health : n2->P2Stats.health < n1->P2Stats.health; };

    Node* x = *std::min_element(_nodes.begin(), _nodes.end(), cmp);

    return _currentTurn == Game::Turn::P1 ? x->P1Stats.health : x->P2Stats.health;
}

