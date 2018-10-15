#pragma once

#include <string>
#include "imgui/imgui.h"
#include "game.h"
#include "playerStats.h"
#include <vector>



class Node
{
    using nodeVec = std::vector<Node*>;

public:
    enum Status{ROOT, MIDDLE, END};
    enum Move{ATTACK, DEFEND, HEAL};
    Node(int id, const ImVec2 &pos, int graphLevel, int inputsCount,
         int outputsCount, Status status); //   input/output nodes are references to vector of node pointers
    Node(int id, Node& parent);
    ~Node() = default;
    ImVec2 getInputSlotPos() const;
    ImVec2 getOutputSlotPos() const;

    inline void addParent(Node *n) { parentNodes.push_back(n); parentCount++; }
    inline void addChildren(Node *n) { childrenNodes.push_back(n); childrenCount++; }
    inline void addParent(nodeVec& nodes);
    inline void addChildren(nodeVec& nodes);
    inline void setPos(ImVec2 pos){ this->pos = pos; }
    inline void setPos(int nthElem, int level, int xSettingsOffset, int ySettingsOffSet)
        { pos = ImVec2(nthElem * xSettingsOffset + size.x, level * ySettingsOffSet + size.y);}
    inline void setStatus(Status status){ _status = status; }
    inline Status getStatus(){ return _status; }
    inline bool isAlive(){ return P1Stats.health > 0 && P2Stats.health > 0; }
    inline bool isSelected(Node* child){ return selected && child->selected; }
    inline PlayerStats* getNextTurnStats(Game::Turn t){ return t == Game::Turn::P1 ? &P2Stats : &P1Stats; }
    inline PlayerStats* getCurrTurnStats(Game::Turn t){ return t == Game::Turn::P1 ? &P1Stats : &P2Stats; }

    void removeChild(Node *child);
    void setHighlighted(bool b);
    const char* getMoveStr();
    void incMove(Game::Turn t);

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
    bool        selected;
    Move        move;


private:
    Status      _status;
};


