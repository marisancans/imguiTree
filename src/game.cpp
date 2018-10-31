#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>




// SPAGHETTI OVERLOAD ~~~~~~  ~~~~  ~~    ~~~~     ~~~
void Game::genLayer()
{
    Turn t = _turn == P1 ? P2 : P1;
    
    Traverse min{_nodes[0][0].interspace, -1};
    Traverse max{0.f, -1};
    
    for(int i = 0; i < 3; ++i) {
        NODE_VEC newLayer;
        
        for(auto& parent : _nodes.back()) { // Last vector (Parent nodes)
            auto parentPos = t == P1 ? parent.P1Pos : parent.P2Pos;
            POS_VEC possPositions = getPossibleMoves(parentPos, t);
            
            // --------------  Append to vector or add to parents ---------
            
            for(auto& pos : possPositions) {
                auto child = Node(&parent);
                child.ID = getNewID();

                auto childPos = t == P1 ? &child.P1Pos : &child.P2Pos;
                childPos->x = pos.x;
                childPos->y = pos.y;
                
                // Check if node already exists
                if(newLayer.empty())
                    newLayer.push_back(child);
                else {
                    for(auto& n : newLayer) {
                        if(n.P1Pos == child.P1Pos && n.P2Pos == child.P2Pos) {
                            parent.childNodes.push_back(child.ID);
                            break;
                        } else {
                            newLayer.push_back(child);
                            break;
                        }
                        
                    }
                }
            }
        }


        
        
        //-----------------   Get minmax interspaces ------------------
        if(!newLayer.empty()) {
            for(auto& n : newLayer)
                n.calcInterspace();
            
            
            for(auto& n : newLayer) {
                if(n.interspace < min.val && t == P1) {
                    min.val = n.interspace;
                    min.ID = n.ID;
                }
                if(n.interspace > max.val && t == P2) {
                    max.val = n.interspace;
                    max.ID = n.ID;
                }
            }
            
        }
        
        
        _nodes.push_back(newLayer);
        t = t == P1 ? P2 : P1;
    }
    
    

    if(_turn == P2) {
        for(int i = int(_nodes.size()); --i >= 1;)
            for(auto& node : _nodes[i]) {
                if (min.ID == node.ID) {
                    min.ID = node.parentNodeID;
                    currPosP1 = node.P1Pos;
                    break;
                }
            }
    }
    

    if(_turn == P1) {
        for(int i = int(_nodes.size()); --i >= 1;)
            for(auto& node : _nodes[i]) {
                if (max.ID == node.ID) {
                    max.ID = node.parentNodeID;
                    currPosP2 = node.P2Pos;
                    break;
                }
            }
    }
    
}

Game::Game(Game::GameMode mode, Game::Turn turn, GameSettings& gameSettings):
    _turn(turn), gameSettings(gameSettings), _newID(0)
{
    init();
}

void Game::init() {
    currPosP1 = gameSettings.P1StartPos;
    currPosP2 = gameSettings.P2StartPos;
    swapTurn();
    makeTurns();
}

void Game::makeTurns() {
    static auto started = std::chrono::high_resolution_clock::now();
    // Clock
    auto done = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();
    if(ms > gameSettings.speedMS) {

        _newID = -1;
        Node n;
        n = Node(getNewID(), Node::ROOT, currPosP1, currPosP2);
        n.calcInterspace();

        tracersP1.emplace_back(currPosP1);
        if(tracersP1.size() > 10)
            tracersP1.pop_front();

        tracersP2.emplace_back(currPosP2);
        if(tracersP2.size() > 10)
            tracersP2.pop_front();

        _nodes.clear();


        NODE_VEC nv;
        nv.push_back(n);
        _nodes.push_back(nv);

        genLayer();
        checkWinner();

        swapTurn();
        started = std::chrono::high_resolution_clock::now();
    }
}

bool Game::checkWinner() {
//    if(currNodeP1.interspace == 0)
//        winner = P1;
//    if(currNodeP2.interspace == 0)
//        winner = P2;
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
    auto pv = getPossibleMoves(currPosP1, P1);
    auto tmp = getPossibleMoves(currPosP2, P2);
    for(auto& v : tmp)
        pv.push_back(v);

//    for(auto& p : pv){
//        p.x += 1;
//        p.y += 1;
//    }

    return pv;
}
