#pragma once

#include <vector>
#include <algorithm>
#include "game.h"
//#include "node.h"
class Node;
class ImVec2;

class Layer {
public:
    inline Layer(Game::Turn turn, int level): _currentTurn(turn), _level(level){};
    inline void addNode(Node* n){ _nodes.push_back(n);}
    inline int getLevel(){ return _level; }
    inline std::vector<Node*>& getNodes(){ return _nodes; }
    bool linkDuplicate(Node* child);
    int getNodeCount(){ return int(_nodes.size()); }
    int  getLowestNodeHP();
    inline Game::Turn getCurrentTurn(){ return _currentTurn; }
    inline std::string getTurnStr(){ return _currentTurn == Game::P1 ? "P2" : "P1"; }


private:
    int _level;
    std::vector<Node*> _nodes;
    Game::Turn _currentTurn;
};
