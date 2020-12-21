#ifndef GRASS_H
#define GRASS_H

#include "Piece.h"

class Grass : public Piece
{
public:
	Grass(World& w, int x, int y) : Piece(w, x, y, '*') {}
};

#endif
