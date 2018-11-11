#include "game.h"
#include "node.h"


#include <iostream>
#include <algorithm>
#include <chrono>
#include <thread>
#include <unordered_set>

namespace game {

GameSettings* gameSettings;
Position currPos[PLAYER_COUNT];
std::deque<Position> tracers[PLAYER_COUNT];
Position lastClicked{-1, -1};
bool won;


PlayerIdx _currPlayer;
std::vector<NODE_VEC> nodes;
std::vector<BackTrack> chosenPath;

int newID;

inline void swapTurn(){ _currPlayer = nextPlayer(_currPlayer); }

void genLayer()
{
    // currPlayer in simulated prediction context, not globally
    PlayerIdx currPlayer = nextPlayer(_currPlayer);
    
    Traverse decision{nodes[0][0].interspace, -1};
    for(int i = 0; i < 3; ++i) {
        NODE_VEC newLayer;
        for(auto& parent : nodes.back()) { // Last vector (Parent nodes)
            auto parentPos = parent.pos[currPlayer];
            POS_VEC possPositions = getPossibleMoves(parentPos, currPlayer);
            
            // --------------  Append to vector or add to parents ---------
            
            for(auto& pos : possPositions) {
                bool found = false;
                // Check if node already exists
                for(auto& n : newLayer) {
                    if(n.pos[currPlayer] == pos) {
                        parent.childNodes.push_back(n.ID);
                        found = true;
                        break;
                    }
                }
                
                if(found)
                    continue;
    
    
                Node child;
                child.ID = ++newID;
                child._status = Node::MIDDLE;
                child.parentNodeID = parent.ID;
                for(int i = 0; i < PLAYER_COUNT; i++)
                    child.pos[i] = parent.pos[i];
    
                auto childPos = &child.pos[currPlayer];
                childPos->x = pos.x;
                childPos->y = pos.y;
                parent.childNodes.push_back(child.ID);
                newLayer.push_back(child);
            }
        }
        
        
        // look for best move depending on player strategy
        if(!newLayer.empty()) {
            for(auto& n : newLayer) {
                n.calcInterspace();
                if(strategies[currPlayer](n.interspace, decision.val)) {
                    decision.val = n.interspace;
                    decision.ID = n.ID;
                }
            }
        }
        
        
        nodes.push_back(newLayer);
        currPlayer = nextPlayer(_currPlayer);
    }
    
    
    
    // @fixme
    chosenPath.clear();
    auto pIdx = nextPlayer(_currPlayer);
    for(int i = int(nodes.size()); --i >= 1;)
        for(auto& node : nodes[i]) {
            if (decision.ID == node.ID) {

//                if(chosenPath.empty())
//                    chosenPath.push_back({node.ID, node.parentNodeID});

                chosenPath.push_back({node.ID, node.parentNodeID});
                decision.ID = node.parentNodeID;
                currPos[pIdx] = node.pos[pIdx];
                break;
            }
        }
}

void init(PlayerIdx turn, GameSettings& gs)
{
    _currPlayer = turn;
    gameSettings = &gs;
    newID = 0;
    for(int i = 0; i < PLAYER_COUNT; i++)
        currPos[i] = gameSettings->startPos[i];

    makeTurns(gs);
}

GameSettings setSettings(GameMode mode){
    won = false;

    GameSettings gameSettings;
    gameSettings.gameMode = mode;
    gameSettings.maxLayer = 3;
    gameSettings.maxBoardX = 20;
    gameSettings.maxBoardY = 20;
    gameSettings.scrolling = ImVec2(0.0f, 0.0f);
    gameSettings.showGrid = false;
    gameSettings.levelOffsetX = 100;
    gameSettings.levelOffsetY = 100;
    gameSettings.speedMS = 500;
    for(int p = 0; p < PLAYER_COUNT; p++)
        for(int i = 0; i < 8; i++)
            gameSettings.movRange[p][i][0] = true;
    for(int i = 0; i < 8; i += 2)
        gameSettings.movRange[P2][i][1] = true;

    gameSettings.startPos[P1] = {0, 0};
    gameSettings.startPos[P2] = {gameSettings.maxBoardX - 1, gameSettings.maxBoardY - 1};
    return gameSettings;
};


void makeTurns(GameSettings& gs) {


    static auto started = std::chrono::high_resolution_clock::now();
    // Clock
    auto done = std::chrono::high_resolution_clock::now();
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(done-started).count();



    newID = 0;
    Node n;
    n.ID = newID++;


    for(int i = 0; i < PLAYER_COUNT; i++)
        n.pos[i] = currPos[i]

                ;
    n.calcInterspace();

    if(!checkWinner(n)) {



        for (int i = 0; i < PLAYER_COUNT; i++) {
            tracers[i].emplace_back(currPos[i]);
            if (tracers[i].size() > 10)
                tracers[i].pop_front();
        }


        NODE_VEC nv;

        if (ms > gs.speedMS && gameSettings->gameMode == PvsPC) {
            nodes.clear();
            if (_currPlayer == P1) {
                nv.push_back(n);
                nodes.push_back(nv);
                genLayer();
                swapTurn();
            } else {
                if (lastClicked.x != -1) {
                    auto laddpos = [&]() {
                        n.pos[P2].x = lastClicked.x;
                        n.pos[P2].y = lastClicked.y;
                        nv.push_back(n);
                        currPos[P1] = n.pos[P2];
                        swapTurn();
                    };

                    if (!gameSettings->hacks) {
                        auto poss = getPossibleMoves(n.pos[P1], P1);

                        for (int i = 0; i < poss.size(); ++i) {
                            if (poss[i] == lastClicked) {
                                laddpos();
                                break;
                            }
                        }
                    } else {
                        laddpos();
                    }
                }
                lastClicked.x = -1;

            }

            started = std::chrono::high_resolution_clock::now();
        } else if (ms > gs.speedMS) {
            nodes.clear();
            nv.push_back(n);
            nodes.push_back(nv);
            genLayer();
            swapTurn();
            started = std::chrono::high_resolution_clock::now();
        }
    }
    else
        won = true;

}


POS_VEC getPossibleMoves(const Position &p, PlayerIdx turn) {
    POS_VEC pv;
    moveMatrix mov{};

    auto canRight = [&](int i){ return p.x + i > 0 && p.x + i < gameSettings->maxBoardX; };
    auto canLeft = [&](int i){ return p.x - i >= 0 && p.x - i < gameSettings->maxBoardX; };
    auto canUp = [&](int i){ return p.y - i >= 0 && p.y - i < gameSettings->maxBoardY; };
    auto canDown = [&](int i){ return p.y + i > 0 && p.y + i < gameSettings->maxBoardY; };
    
    auto rmovRange = gameSettings->movRange[turn];
    
    // Up
    for(int i = 1; i < MRR + 1; ++i){
        if(canUp(i) && rmovRange[0][i - 1]) {
            mov[NORTH][i] = 1;
            pv.push_back(Position{p.x, p.y - i});
        }
        
        if(canRight(i) && canUp(i) && rmovRange[1][i - 1]) {
            mov[NORTH_EAST][i] = 1;
            pv.push_back(Position{p.x + i, p.y - i});
        }
        
        if(canRight(i) && rmovRange[2][i - 1]) {
            mov[EAST][i] = 1;
            pv.push_back(Position{p.x + i, p.y});
        }
        
        if(canRight(i) && canDown(i) && rmovRange[3][i - 1]) {
            mov[SOUTH_EAST][i] = 1;
            pv.push_back(Position{p.x + i, p.y + i});
        }
        
        if(canDown(i) && rmovRange[4][i - 1]) {
            mov[SOUTH][i] = 1;
            pv.push_back(Position{p.x, p.y + i});
        }
        
        if(canLeft(i) && canDown(i) && rmovRange[5][i - 1]) {
            mov[SOUTH_WEST][i] = 1;
            pv.push_back(Position{p.x - i, p.y + i});
        }
        
        if(canLeft(i) && rmovRange[6][i - 1]) {
            mov[WEST][i] = 1;
            pv.push_back(Position{p.x - i, p.y});
        }
        
        if(canLeft(i) && canUp(i) && rmovRange[7][i - 1]) {
            mov[NORTH_WEST][i] = 1;
            pv.push_back(Position{p.x - i, p.y - i});
        }
        
    }
    
    return pv;
}

POS_VEC getRanges() {
    auto pv = getPossibleMoves(currPos[P1], P1);
    auto tmp = getPossibleMoves(currPos[P2], P2);
    for(auto& v : tmp)
        pv.push_back(v);
    
    
    return pv;
}
}