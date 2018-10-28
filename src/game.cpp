#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>



void Game::genLayer() {
    for(int i = 0; i < gameSettings.maxLayer; ++i) {
        NODE_VEC newLayer;

        for (auto parent : _nodes.back()) { // Last vector (Parent nodes)
            auto parentPos = _turn == P1 ? &parent.P1Pos : &parent.P2Pos;
            POS_VEC possPositions = getPossibleMoves(*parentPos);

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

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings gameSettings):
    _turn(turn), gameSettings(gameSettings)
{
    init();
}

void Game::init() {
    // Initial layer creation
    _currNode = Node(0, Node::ROOT, gameSettings.P1StartPos, gameSettings.P2StartPos);
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

    std::this_thread::sleep_for(std::chrono::milliseconds(gameSettings.speedMS));
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
    double min = std::max(gameSettings.maxBoardX, gameSettings.maxBoardY);


    for(auto& layer : _nodes){
        Node mi = *std::min_element(layer.begin(), layer.end(),
                                    [](Node& a, Node& b){ return a.interspace < b.interspace; });
        if(mi.interspace < min)
            min = mi.interspace;
    }
    return min;
}

POS_VEC Game::getPossibleMoves(const Position &p) {
    POS_VEC pv;
    moveMatrix mov{};

    auto canRight = [&](int i){ return p.x + i > 0 && p.x + i < gameSettings.maxBoardX; };
    auto canLeft = [&](int i){ return p.x - i >= 0 && p.x - i < gameSettings.maxBoardX; };
    auto canUp = [&](int i){ return p.y - i >= 0 && p.y - i < gameSettings.maxBoardY; };
    auto canDown = [&](int i){ return p.y + i > 0 && p.y + i < gameSettings.maxBoardY; };

    const moveMatrix& rmovRange = _turn == P1 ? gameSettings.P1MovRange : gameSettings.P2MovRange;

    // Up
    for(int i = 1; i < MRR + 1; ++i){
        if(canUp(i) && rmovRange[0][i - 1]) {
            mov[UP][i] = 1;
            pv.push_back(Position{p.x, p.y - i});
        }

        if(canRight(i) && canUp(i) && rmovRange[1][i - 1]) {
            mov[UP_RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y - i});
        }

        if(canRight(i) && rmovRange[2][i - 1]) {
            mov[RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y});
        }

        if(canRight(i) && canDown(i) && rmovRange[3][i - 1]) {
            mov[DOWN_RIGHT][i] = 1;
            pv.push_back(Position{p.x + i, p.y + i});
        }

        if(canDown(i) && rmovRange[4][i - 1]) {
            mov[DOWN][i] = 1;
            pv.push_back(Position{p.x, p.y + i});
        }

        if(canLeft(i) && canDown(i) && rmovRange[5][i - 1]) {
            mov[DOWN_LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y + i});
        }

        if(canLeft(i) && rmovRange[6][i - 1]) {
            mov[LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y});
        }

        if(canLeft(i) && canUp(i) && rmovRange[7][i - 1]) {
            mov[UP_LEFT][i] = 1;
            pv.push_back(Position{p.x - i, p.y - i});
        }

    }

    return pv;
}
