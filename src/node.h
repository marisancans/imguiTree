#pragma once

#include <string>
#include "imgui/imgui.h"
#include <vector>



class Node
{
    using nodeVec = std::vector<Node*>;

public:
    enum Status{ROOT, MIDDLE, END};
    Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
         int health, int attack, int defense, int heal, int inputsCount, int outputsCount, Status status); //   input/output nodes are references to vector of node pointers
    Node(Node& parent, int id, std::string name);


    ImVec2 getInputSlotPos() const;
    ImVec2 getOutputSlotPos() const;
    inline const char* getCName() { return name.c_str(); }
    inline void addParent(Node *n) { parentNodes.push_back(n); parentCount++; }
    inline void addChildren(Node *n) { childrenNodes.push_back(n); childrenCount++; }
    inline void addParent(nodeVec& nodes);
    inline void addChildren(nodeVec& nodes);
    inline void setPos(ImVec2 pos){ this->pos = pos; }
    inline bool isAlive(){ return bool(health); }

    bool operator!=(Node const& parent);

    int                 id;
    std::string         name;
    ImVec2              pos;
    int                 graphLevel;
    ImVec2              size;
    int                 health;
    int                 attack;
    int                 defense;
    int                 heal;
    nodeVec             parentNodes;
    nodeVec             childrenNodes;
    int                 parentCount;
    int                 childrenCount;
    Status              status;
};


