#pragma once

#include <vector>

struct Position;

const int MRR = 3; // Max movement range
using POS_VEC = std::vector<Position>;

enum MovDir // Movement directions
{
    UP = 0,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT ,
    LEFT,
    UP_LEFT
};




class Board {
public:
    using moveMatrix = int[8][MRR];
    inline Board(int xMax, int yMax, const moveMatrix& P1MovRange,  const moveMatrix& P2MovRange):
        _xMax(xMax), _yMax(yMax), _P1MovRange(P1MovRange), _P2MovRange(P2MovRange) {}
    POS_VEC getPossibleMoves(const Position& p);

private:

    
    int _xMax;
    int _yMax;
    const moveMatrix& _P1MovRange; // Movement ranges
    const moveMatrix& _P2MovRange;
    
};
