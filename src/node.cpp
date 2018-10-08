#include "node.h"


Node::Node(int id, std::string const& name, const ImVec2 &pos, int graphLevel,
          int health, int attack, int defense, int heal, int inputsCount, int outputsCount) :
           id(id), name(name), pos(pos), graphLevel(graphLevel), health(health), attack(attack),
           defense(defense), heal(heal), inputsCount(inputsCount), outputsCount(outputsCount)
    {

    }

ImVec2 Node::getInputSlotPos() const
{
    return ImVec2(pos.x + size.x  / ((float)inputsCount + 1), + pos.y);
}

ImVec2 Node::getOutputSlotPos() const
{
    return ImVec2(pos.x + size.x / ((float)outputsCount + 1), pos.y + size.y);
}

Node::Node(const Node& parent, int id, std::string name) {
    this->id = id;
    this->name = name;
    pos = ImVec2(0, 0);
    graphLevel = parent.graphLevel + 1;
    health = parent.health;
    attack = parent.attack;
    defense = parent.defense;
    heal = parent.heal;
    inputsCount = 1;
    outputsCount = 0;
}




