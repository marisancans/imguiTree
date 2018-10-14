#include <algorithm>
#include "node.h"


Node::Node(int id, const ImVec2 &pos, int graphLevel, int inputsCount,
           int outputsCount, Status status) :
           id(id), pos(pos), graphLevel(graphLevel),parentCount(inputsCount),
           childrenCount(outputsCount), _status(status)
    {

    }

ImVec2 Node::getInputSlotPos() const
{
    return ImVec2(pos.x + size.x  / 2, + pos.y);
}

ImVec2 Node::getOutputSlotPos() const
{
    return ImVec2(pos.x + size.x / 2, pos.y + size.y);
}

Node::Node(int id, Node& parent) {
    this->id = id;
    graphLevel = parent.graphLevel + 1;
    this->P1Stats = parent.P1Stats;
    this->P2Stats = parent.P2Stats;
    parentCount = 1;
    childrenCount = 0;
    _status = MIDDLE;
}

void Node::addParent(Node::nodeVec &nodes) {
    parentNodes.insert(parentNodes.end(), nodes.begin(), nodes.end());
    parentCount += nodes.size();
}

void Node::addChildren(Node::nodeVec &nodes) {
    childrenNodes.insert(childrenNodes.end(), nodes.begin(), nodes.end());
    childrenCount += nodes.size();
}

void Node::removeChild(Node *child) {
    childrenNodes.erase(
            std::remove_if(childrenNodes.begin(), childrenNodes.end(),
                           [&child](Node* n) { return child == n; }), childrenNodes.end());

}

void Node::setSelected(bool b) {
    if(b) {
        for (auto &p : parentNodes)
            p->setSelected(true);
        selected = true;
    } else {
        selected = false;
    }
}

const char *Node::getMoveStr(){
    switch(move){
        case ATTACK: return std::string("Attack").c_str();
        case DEFEND: return std::string("Defend").c_str();
        case HEAL:   return std::string("Heal").c_str();
        default:     return std::string("Root").c_str();
    }
}

void Node::incMove(Game::Turn t) {
    PlayerStats* s = getCurrTurnStats(t);
    switch(move){
        case ATTACK:
           s->attackCount++;
           if(s->defenseCount != 0)
            s->defenseCount--;
           if(s->healCount != 0)
                s->healCount--;
           break;
        case DEFEND:
            s->defenseCount++;
            if(s->attackCount != 0)
                s->attackCount--;
            if(s->healCount != 0)
                s->healCount--;
            break;
        case HEAL:
            s->healCount++;
            if(s->attackCount != 0)
                s->attackCount--;
            if(s->defenseCount != 0)
                s->defenseCount--;
            break;
    }


    getCurrTurnStats(t);
}


