#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>
#include "board.h"
#include <iostream>

struct Position {
    int x;
    int y;

    inline bool operator ==(const Position & obj) const
    {
        return x == obj.x && y == obj.y;
    }
};

template<>
struct std::hash<Position>
{
    size_t operator()(const Position & obj) const
    {
        return hash<int>()(1/2*(obj.x + obj.y)*(obj.x + obj.y + 1) + obj.y);
    }
};

class Node
{
public:
    using NODE_VEC = std::vector<Node*>;
    enum Status{ROOT = 1, MIDDLE = 2, END = 3};
    inline Node(int id, Status status, Position P1StartPos, Position P2StartPos):
        _id(id), _status(status), P1Pos(P1StartPos), P2Pos(P2StartPos){};
    inline Node(Status status) : _status(status) {};
    Node(int id, Node const* parent);
    ~Node() = default;

    void setHighlighted(bool b);

    std::vector<int> parentNodes;
    std::vector<int> childNodes;
    Position         P1Pos;
    Position         P2Pos;
    int              interspace; //Distance P1 <---> P2
    bool             selected;

private:
    int        _id;
    Status     _status;
};


