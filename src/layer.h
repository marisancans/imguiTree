#pragma once

#include <vector>
//#include "node.h"
class Node;
class ImVec2;
class Layer {
public:
    inline Layer(int level): _level(level){};
    inline void addNode(Node* n){ _nodes.push_back(n);}
    inline int getLevel(){ return _level; }
    inline std::vector<Node*>& getNodes(){ return _nodes; }
    bool linkDuplicate(Node* child);
    int getNodeCount(){ return int(_nodes.size()); }

private:
    int _level;
    std::vector<Node*> _nodes;
};
