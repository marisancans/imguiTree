#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>



void Game::genLayer() {
    Turn t = _turn;
    for(int i = 0; i < 2; ++i) {
        NODE_VEC newLayer;

        for (auto parent : _nodes.back()) { // Last vector (Parent nodes)
            auto parentPos = t == P1 ? &parent.P1Pos : &parent.P2Pos;
            POS_VEC possPositions = getPossibleMoves(*parentPos, t);

            int nth = 0;
            for (auto &pos : possPositions) {
                auto child = Node(nth++, &parent);
                auto childPos = t == P1 ? &child.P1Pos : &child.P2Pos;
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

                            newLayer.push_back(child);
                            break;
                        }

                    }
                }




            }
        }

        _nodes.push_back(newLayer);
        t = t == P1 ? P2 : P1;

    }
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings gameSettings):
    _turn(turn), gameSettings(gameSettings)
{
    init();
}

void Game::init() {
    // Initial layer creation
    auto n = Node(0, Node::ROOT, gameSettings.P1StartPos, gameSettings.P2StartPos);
    n.calcInterspace();
    NODE_VEC nv;
    nv.push_back(n);
    _nodes.push_back(nv);


    currNodeP1 = &_nodes[0][0];
    currNodeP2 = &_nodes[0][0];



    swapTurn();
}

void Game::makeTurns() {


//            if (i < turns - 1) {
    _nodes.clear();

    auto n = Node(0, Node::ROOT, currNodeP1->P1Pos, currNodeP2->P2Pos);
    n.calcInterspace();
    NODE_VEC nv;
    nv.push_back(n);
    _nodes.push_back(nv);
//            }

    genLayer();
    minMaxInterspace();
    checkWinner();



    std::this_thread::sleep_for(std::chrono::milliseconds(gameSettings.speedMS));
    swapTurn();
//}



}

bool Game::checkWinner() {

}


void Game::minMaxInterspace() {
    _nodes.back().back().calcInterspace();
    double min = _nodes.back().back().interspace;
    double max = 0.f;

    // Note the prefix ++ for _nodes, that means we start from second element
    for(auto itLayer = ++_nodes.begin(); itLayer != _nodes.end(); ++itLayer){
        for(auto& n : *itLayer) {
            n.calcInterspace();
            if(n.interspace < min && _turn == P1) {
                min = n.interspace;
                currNodeP1 = &n;
                n.selected = 1;
            }
            if(n.interspace > max && _turn == P2) {
                max = n.interspace;
                currNodeP2 = &n;
                n.selected = 2;
            }
        }
    }
fix gme ends when interspace = 0
}

POS_VEC Game::getPossibleMoves(const Position &p, Turn turn) const{
    POS_VEC pv;
    moveMatrix mov{};

    auto canRight = [&](int i){ return p.x + i > 0 && p.x + i < gameSettings.maxBoardX; };
    auto canLeft = [&](int i){ return p.x - i >= 0 && p.x - i < gameSettings.maxBoardX; };
    auto canUp = [&](int i){ return p.y - i >= 0 && p.y - i < gameSettings.maxBoardY; };
    auto canDown = [&](int i){ return p.y + i > 0 && p.y + i < gameSettings.maxBoardY; };

    const moveMatrix& rmovRange = turn == P1 ? gameSettings.P1MovRange : gameSettings.P2MovRange;

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

POS_VEC Game::getRanges() const{
    auto pv = getPossibleMoves(currNodeP1->P1Pos, P1);
    auto tmp = getPossibleMoves(currNodeP2->P2Pos, P2);
    for(auto& v : tmp)
        pv.push_back(v);

//    for(auto& p : pv){
//        p.x += 1;
//        p.y += 1;
//    }

    return pv;
}
