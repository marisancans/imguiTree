#include "node.h"
#include "game.h"
#include "imgui/imgui.h"

void Game::getMoves(Node* n) {

}

Game::Game(Game::GameMode mode, Game::Turn turn) {
    init();
    initMoves();
}

void Game::init() {
    auto inputs = [this](){ return _turn == P1 ? 0 : 1; };
    Node* n = new Node(int(_nodes.size()), genName(), ImVec2(0, 0), 0, 10, 3, 2, 2, inputs(), 0);
    getMoves(n);
}

void Game::initMoves() {

}

void Game::attack(Node* parrent){

}

void Game::defend(Node* parrent){

}

void Game::heal(Node* parrent){

}

