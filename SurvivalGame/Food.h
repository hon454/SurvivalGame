#ifndef FOOD_H
#define FOOD_H

#include "Piece.h"

class Food : public Piece
{
public:
	Food(World& w, int x, int y) : Piece(w, x, y, 'F') {}
};

#endif