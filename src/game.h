#pragma once

#include <vector>
#include <string>

class Layer;
class Node;

class Game {
public:
    enum GameMode{ PvsPC, PCvsPC};
    enum Turn{P1, P2};
    Game(GameMode mode, Turn turn);
    void getNextLayer();
    inline const std::vector<Layer*>& getLayers(){ return _layers; }
    inline std::string genName(){ return _turn == P1 ? "P1" : "P2"; }
    inline void swapTurn(){ _turn = _turn == P1 ? P2 : P1; }
    Node* createChild(Node* parent);
    Node* attack(Node* parent);
    Node* defend(Node* parent);
    Node* heal(Node* parent);

private:
    GameMode _mode;
    Turn _turn;
    int _nodeCount;
    std::vector<Layer*> _layers;
    std::vector<Node* (Game::*)(Node* n)> _moves;

    void init();
    void initMoves();
    int getNodeID() { return ++_nodeCount;}


};
