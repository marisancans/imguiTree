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

                        // Check if next move is possible
                        bool np = false;
                        PlayerStats* s = child->getCurrTurnStats(_turn);
                        PlayerStats const* i = &gameSettings->initPlayerStats;
                        switch(child->move) {
                            case Node::ATTACK:
                                np = s->attackCount < i->attack;
                                break;
                            case Node::DEFEND:
                                np = s->defenseCount < i->defense;
                                break;
                            case Node::HEAL:
                                np = s->healCount < i->heal;
                                break;
                        }


                        if (child->getStatus() != Node::END && !contain && np) {
                            child->addParent(parent);
                            l->addNode(child);
                            child->setHighlighted(true);
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
    auto l = new Layer(ROOT, 0);
    auto n = new Node(getNodeID(), ImVec2(0, 0), 0, 0, 0, Node::ROOT);
    n->P1Stats = gameSettings->initPlayerStats;
    n->P2Stats = gameSettings->initPlayerStats;
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
    child->move = Node::ATTACK;
    int pDef = 0;
    if(parent->move == Node::DEFEND)
        pDef = parent->getCurrTurnStats(_turn)->defense - parent->getCurrTurnStats(_turn)->defenseCount;
    child->getNextTurnStats(_turn)->health -=
            parent->getCurrTurnStats(_turn)->attack - child->getCurrTurnStats(_turn)->attackCount - pDef;
    child->incMove(_turn);

    return child;
}

Node* Game::defend(Node* parent){
    auto child = createChild(parent);
    child->move = Node::DEFEND;
    child->incMove(_turn);
    return child;
}

Node* Game::heal(Node* parent){
    auto child = createChild(parent);
    child->move = Node::HEAL;
    // Following statement is to prevent infinite healing
    if (child->getCurrTurnStats(_turn)->health >= parent->getNextTurnStats(_turn)->health)
        return attack(parent);

    child->getCurrTurnStats(_turn)->health +=
            child->getCurrTurnStats(_turn)->heal -
            child->getCurrTurnStats(_turn)->healCount;
    child->incMove(_turn);

    return child;
}


