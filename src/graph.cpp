//
// Created by maris on 10/7/18.
//

#include "graph.h"
#include "node.h"


Graph::Graph(bool showGrid) : showGrid(showGrid){
    scrolling = ImVec2(0.0f, 0.0f);
}

void Graph::addNode(Node *n)
{
    if(layout.find(n->graphLevel) == layout.end())
    {   //Not found
        layout.insert(level(n->graphLevel, std::vector<Node*>{n}));
    } else {
        auto it = layout.find(n->graphLevel);
        if (it != layout.end())
            it->second.push_back(n);
    }
//    layout.insert(std::pair<int, std::vector<Node*>>(n->graphLevel, n));
}

void Graph::addNode(std::vector<Node *> &nodes) {
    for(auto const& n : nodes)
        addNode(n);
}

