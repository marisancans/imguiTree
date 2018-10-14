#pragma once

#include <vector>
#include <string>
#include "imgui/imgui.h"

struct GameSettings{
    int layerCount;
    ImVec2 scrolling;
    bool showGrid;
    int levelOffsetYTo;
    int levelOffsetXTo;
    int speedMS;
};

class Layer;
class Node;

class Game {
public:
    enum GameMode{ PvsPC, PCvsPC};
    enum Turn{P1, P2};
    Game(GameMode mode, Turn turn, GameSettings* gameSettings);
    void getNextLayer();
    inline int getLayerCount() const { return _layerCount; };
    inline const std::vector<Layer*>& getLayers() const { return _layers; }
    inline std::string genName(){ return _turn == P1 ? "P1" : "P2"; }
    inline void swapTurn(){ _turn = _turn == P1 ? P2 : P1; }
    Node* createChild(Node* parent);
    Node* attack(Node* parent);
    Node* defend(Node* parent);
    Node* heal(Node* parent);
    GameSettings* gameSettings;

private:

    GameMode _mode;
    Turn _turn;
    int _nodeCount;
    int _layerCount;
    std::vector<Layer*> _layers;
    std::vector<Node* (Game::*)(Node* n)> _moves;



    void init();
    void initMoves();
    int getNodeID() { return ++_nodeCount;}
    Layer* getLastLayer(){ return _layers.back(); }

};
