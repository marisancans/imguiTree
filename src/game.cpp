#include "game.h"
#include "node.h"
#include "layer.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>

void Game::getNextLayer() {
        for(int i = 0; i < gameSettings->layerCount; ++i) {
            auto l = new Layer(_turn, ++_layerCount);
            auto prevNodes = _layers.back()->getNodes();
            _layers.push_back(l);



            for (auto &parent : prevNodes) {
                if(parent->getStatus() != Node::END) {

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


                        if (child->getStatus() != Node::END && !contain) {
                            child->addParent(parent);
                            l->addNode(child);
                            child->setSelected(true);
                        }
                        if (!child->isAlive())
                            child->setStatus(Node::END);

                    }
                }
            }

            swapTurn();

        }
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _nodeCount(0), _turn(turn), gameSettings(gameSettings), _layerCount(0)
{
    initMoves();
    init();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    auto l = new Layer(_turn, 0);
    auto n = new Node(getNodeID(), ImVec2(0, 0), 0, inputs(), 0, Node::ROOT);
    n->P1Stats = {6, 3, 2, 2};// TODO reduce attack each time ,aka add prev move hisotry
    n->P2Stats = {6, 3, 2, 2};// TODO sould be passed to game from main, gameSettings
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
    child->getNextTurnStats(_turn)->health -= parent->getCurrTurnStats(_turn)->attack;

    return child;
}

Node* Game::defend(Node* parent){
    auto child = createChild(parent);
    child->getNextTurnStats(_turn)->health -= parent->getCurrTurnStats(_turn)->attack - child->getNextTurnStats(_turn)->defense;
    return child;
}

Node* Game::heal(Node* parent){
    auto child = createChild(parent);
    // Following statement is to prevent infinite healing
    if (child->getCurrTurnStats(_turn)->health >= parent->getNextTurnStats(_turn)->health)
        return attack(parent);
    child->getCurrTurnStats(_turn)->health += child->getCurrTurnStats(_turn)->heal ;

    return child;
}


