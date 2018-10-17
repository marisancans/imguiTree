#pragma once

//#include "node.h"

#include "node.h"
#include "global.h"

const int MRR = 3; // Max movement range
const int MMC = 8; // Max movement count

enum MovDir // Movement directions
{
    UP = 1,
    UP_RIGHT = 2,
    RIGHT = 3,
    DOWN_RIGHT = 4,
    DOWN = 5,
    DOWN_LEFT = 6,
    LEFT = 7,
    UP_LEFT = 8
};

// Defines how far a player can go in a direction
struct Movement
{
    int up[MRR];
    int upRight[MRR];
    int right[MRR];
    int downRight[MRR];
    int down[MRR];
    int downLeft[MRR];
    int left[MRR];
    int upLeft[MRR];
};

_moves.push_back(&Board::moveUp);
_moves.push_back(&Board::moveUpRight);
_moves.push_back(&Board::moveRight);
_moves.push_back(&Board::moveDownRight);
_moves.push_back(&Board::moveDown);
_moves.push_back(&Board::moveDownLeft);
_moves.push_back(&Board::moveLeft);
_moves.push_back(&Board::moveUpLeft);


struct Position;
class Board {
public:
    inline Board(int xMax, int yMax, Movement const& P1MovRange,  Movement const& P2MovRange):
        _xMax(xMax), _yMax(yMax), _P1MovRange(P1MovRange), _P2MovRange(P2MovRange) {}
    std::vector<Movement> getPossibleMoves(Node const* parent);

private:
    std::vector<Movement* (Board::*)(Movement* n)> _moves;
    void initMoves();
    void moveUp(Position P1PrevMove, Position P2PrevMove);
    )
    
    
    int _xMax;
    int _yMax;
    Movement const& _P1MovRange; // Movement ranges
    Movement const& _P2MovRange;
    
};
