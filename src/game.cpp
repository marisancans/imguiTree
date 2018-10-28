#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>



void Game::genLayer() {
    for(int i = 0; i < gameSettings->maxLayer; ++i) {
        NODE_VEC newLayer;

        for (auto parent : _nodes.back()) { // Last vector (Parent nodes)
            auto parentPos = _turn == P1 ? &parent.P1Pos : &parent.P2Pos;
            POS_VEC possPositions = _board->getPossibleMoves(*parentPos);

            int nth = 0;
            for (auto &pos : possPositions) {
                auto child = Node(nth++, &parent);
                auto childPos = _turn == P1 ? &child.P1Pos : &child.P2Pos;
                childPos->x = pos.x;
                childPos->y = pos.y;

                if(newLayer.empty())
                    newLayer.push_back(child);
                else {
                    for (auto& n : newLayer) {
//                        if(n == NULL)
//                            child.calcInterspace();
                        if (n.P1Pos == child.P1Pos && n.P2Pos == child.P2Pos) {
                            n.parentNodes.push_back(parent.getID());
                            break;
                        } else {
                            child.calcInterspace();
                            newLayer.push_back(child);
                            break;
                        }

                    }
                }




            }
        }

        _nodes.push_back(newLayer);

        swapTurn();
    }
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings* gameSettings):
    _turn(turn), gameSettings(gameSettings)
{
    init();
}

void Game::init() {
    _board = new Board(gameSettings->maxBoardX, gameSettings->maxBoardY,
                       gameSettings->P1MovRange, gameSettings->P2MovRange);

    // Initial layer creation
    _currNode = Node(0, Node::ROOT, gameSettings->P1StartPos, gameSettings->P2StartPos);
    NODE_VEC nv;
    nv.push_back(_currNode);
    _nodes.push_back(nv);

    swapTurn();


}

void Game::makeTurns() {
    for(int i = 0; i < 1; ++i){
        genLayer();
        if(_turn == P1)
            auto max = getMaxInterspace();
        checkWinner();
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(gameSettings->speedMS));
}

bool Game::checkWinner() {

}

double Game::getMaxInterspace() {
    double max = 0.f;

    for(auto& layer : _nodes){
        Node ma = *std::max_element(layer.begin(), layer.end(),
                                    [](Node& a, Node& b){ return a.interspace > b.interspace; });
        if(ma.interspace > max)
            max = ma.interspace;
    }

    return max;
}

double Game::getMinInterspace() {
    double min = std::max(gameSettings->maxBoardX, gameSettings->maxBoardY);


    for(auto& layer : _nodes){
        Node mi = *std::min_element(layer.begin(), layer.end(),
                                    [](Node& a, Node& b){ return a.interspace < b.interspace; });
        if(mi.interspace < min)
            min = mi.interspace;
    }
    return min;
}
