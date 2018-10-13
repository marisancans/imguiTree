#include "game.h"
#include "node.h"
#include "layer.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

void Game::getNextLayer() {
        for(int i = 0; i < gameSettings->layerCount; ++i) {
            auto l = new Layer(_layers.back()->getLevel() + 1);
            auto prevNodes = _layers.back()->getNodes();
            _layers.push_back(l);

            for (auto &parent : prevNodes) {
                for (auto &fn : _moves) {
                    auto child = (this->*fn)(parent);
                    std::this_thread::sleep_for(std::chrono::milliseconds(gameSettings->speedMS));
                    // Goes through all layer nodes to find match, if so, cleanup references
                    bool contain = false;
                    for (auto &n : l->getNodes()) {
                        if (n->P1Stats == child->P1Stats && n->P2Stats == child->P2Stats) {
                            n->addParent(parent);
                            parent->removeChild(child);
                            contain = true;
                        }
                    }


                    if (child->isAlive() && !contain) {
                        child->addParent(parent);
                        l->addNode(child);
                        child->setSelected(true);
                    } else {
                        child->setStatus(Node::END);
                        // return;
                    }
                }
            }

            swapTurn();

        }
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), gameSettings(gameSettings)
{
    initMoves();
    init();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    auto l = new Layer(0);
    auto n = new Node(getNodeID(), ImVec2(0, 0), 0, inputs(), 0, Node::ROOT);
    n->P1Stats = {4, 3, 2, 2};
    n->P2Stats = {4, 3, 2, 2};
    l->addNode(n);
    _layers.push_back(l);
}

void Game::initMoves() {
    _moves.push_back(&Game::attack);
    _moves.push_back(&Game::defend);
    _moves.push_back(&Game::heal);
}

Node *Game::createChild(Node *parent) {
    Node* child = new Node(getNodeID(), *parent);
    int level = getLastLayer()->getLevel();
    int nodeCount =  getLastLayer()->getNodeCount();
    int x = gameSettings->levelOffsetXTo;
    int y = gameSettings->levelOffsetYTo;

    child->setPos(nodeCount, level, x, y);
    return child;
}

Node* Game::attack(Node* parent){
    auto child = createChild(parent);
//    child.health -= parent->attack;
    if(_turn == P1)
        child->P2Stats.health -= parent->P1Stats.attack;
    else
        child->P1Stats.health -= parent->P2Stats.attack;

    return child;
}

Node* Game::defend(Node* parent){
    auto child = createChild(parent);
    if(_turn == P1)
        child->P2Stats.health -= parent->P1Stats.attack - child->P2Stats.defense;
    else
        child->P1Stats.health -= parent->P2Stats.attack - child->P2Stats.defense;
    return child;
}

Node* Game::heal(Node* parent){
    auto child = createChild(parent);
    if(_turn == P1)
        child->P1Stats.health += child->P1Stats.heal;
    else
        child->P2Stats.health += child->P2Stats.heal;
    return child;
}


