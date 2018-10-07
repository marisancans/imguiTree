#pragma once

#include <vector>
#include <string>

class Node;

class Game {
public:
    enum GameMode{ PvsPC, PCvcPC};
    enum Turn{P1, P2};
    Game(GameMode mode, Turn turn);
    void getMoves(Node* n);
    inline std::string genName(){ return _turn == P1 ? "P1" : "P2"; }



private:
    GameMode _mode;
    Turn _turn;
    std::vector<Node*> _nodes;
    std::vector<void (*)(Node* n)> _moves;

    void init();
    void initMoves();
    void attack(Node* parrent);
    void defend(Node* parrent);
    void heal(Node* parrent);

};
