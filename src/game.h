#pragma once

#include <vector>
#include <string>

class Node;

class Game {
public:
    enum GameMode{ PvsPC, PCvsPC};
    enum Turn{P1, P2};
    Game(GameMode mode, Turn turn);
    void getLevel(Node *n);
    inline std::vector<Node*>& getNodes(){ return _nodes; }
    inline std::string genName(){ return _turn == P1 ? "P1" : "P2"; }

    Node& createChild(Node* parent);
    Node& attack(Node* parent);
    Node& defend(Node* parent);
    Node& heal(Node* parent);

private:
    GameMode _mode;
    Turn _turn;
    int _graphLevel;
    std::vector<Node*> _nodes;
    std::vector<Node& (Game::*)(Node* n)> _moves;

    void init();
    void initMoves();


};
