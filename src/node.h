#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>
#include <iostream>
#include <cmath>

class Node;

struct Position {
    int x;
    int y;

    bool operator ==(const Position & obj) const
    {
        return x == obj.x && y == obj.y;
    }
};


class Node
{
public:
    using NODE_VEC = std::vector<Node*>;
    enum Status{ROOT = 1, MIDDLE = 2, END = 3};
    inline Node(int id, Status status, Position P1StartPos, Position P2StartPos):
        _id(id), _status(status), P1Pos(P1StartPos), P2Pos(P2StartPos), selected(0){};
    inline Node(Status status) : _status(status) {};
    inline int getID(){ return _id; };
    inline bool operator ==(const Node& obj) const { return P1Pos == obj.P1Pos && P2Pos == obj.P2Pos; }
    inline void calcInterspace(){
        interspace = std::hypot(P2Pos.x-P1Pos.x, P2Pos.y-P1Pos.y);};
    Node(int id, Node const* parent);
    inline Node():_status(ROOT), _id(0), selected(0){};
//    inline bool compareStats(Position childPos){ return p1 == P1Pos && p2 == P2Pos; }

    ~Node() = default;

    void setHighlighted(bool b);

    std::vector<int> parentNodes;
    std::vector<int> childNodes;
    Position         P1Pos;
    Position         P2Pos;
    double           interspace; //Distance P1 <---> P2
    int              selected;

private:
    int        _id;
    Status     _status;
};


