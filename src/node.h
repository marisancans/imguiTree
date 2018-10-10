#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>

struct PlayerStats {
    int health;
    int attack;
    int defense;
    int heal;

    inline bool operator==(PlayerStats const& parent){
        return health == parent.health;
    }
};

class Node
{
    using nodeVec = std::vector<Node*>;

public:
    enum Status{ROOT, MIDDLE, END};
    Node(int id, const ImVec2 &pos, int graphLevel, int inputsCount,
         int outputsCount, Status status); //   input/output nodes are references to vector of node pointers
    Node(int id, Node& parent);
    ~Node() = default;
    ImVec2 getInputSlotPos() const;
    ImVec2 getOutputSlotPos() const;
    //inline const char* getCName() { return name.c_str(); }
    inline void addParent(Node *n) { parentNodes.push_back(n); parentCount++; }
    inline void addChildren(Node *n) { childrenNodes.push_back(n); childrenCount++; }
    inline void addParent(nodeVec& nodes);
    inline void addChildren(nodeVec& nodes);
    inline void setPos(ImVec2 pos){ this->pos = pos; }
    inline void setStatus(Status status){ _status = _status; }
    inline bool isAlive(){ return P1Stats.health > 0 && P2Stats.health > 0; }
    void removeChild(Node *child);


    int         id;
    ImVec2      pos;
    int         graphLevel;
    ImVec2      size;
    nodeVec     parentNodes;
    nodeVec     childrenNodes;
    int         parentCount;
    int         childrenCount;
    PlayerStats P1Stats;
    PlayerStats P2Stats;

private:
    Status      _status;
};


