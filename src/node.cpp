#include "node.h"


Node::Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
          int health, int attack, int defense, int heal, int inputsCount, int outputsCount, Status status) :
           id(id), name(name), pos(pos), graphLevel(graphLevel), health(health), attack(attack),
           defense(defense), heal(heal), parentCount(inputsCount), childrenCount(outputsCount), _status(status)
    {

    }

ImVec2 Node::getInputSlotPos() const
{
    return ImVec2(pos.x + size.x  / ((float)parentCount + 1), + pos.y);
}

ImVec2 Node::getOutputSlotPos() const
{
    return ImVec2(pos.x + size.x / ((float)childrenCount + 1), pos.y + size.y);
}

Node::Node(int id, Node& parent, std::string name) {
    this->id = id;
    this->name = name;
    pos = ImVec2(0, 0);
    graphLevel = parent.graphLevel + 1;
    health = parent.health;
    attack = parent.attack;
    defense = parent.defense;
    heal = parent.heal;
    parentCount = 1;
    childrenCount = 0;
    _status = MIDDLE;

    addParent(&parent);
    parent.addChildren(this);
}

bool Node::operator!=(const Node& parent) {
    return health  != parent.health &&
           attack  != parent.attack &&
           defense != parent.defense &&
           heal    != parent.heal;
}

void Node::addParent(Node::nodeVec &nodes) {
    parentNodes.insert(parentNodes.end(), nodes.begin(), nodes.end());
    parentCount += nodes.size();
}

void Node::addChildren(Node::nodeVec &nodes) {
    childrenNodes.insert(childrenNodes.end(), nodes.begin(), nodes.end());
    childrenCount += nodes.size();
}


