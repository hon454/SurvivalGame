#pragma once

#include "Piece.h"

class Critter : public Piece
{
public:
	Critter(World& w, int x, int y, char initial, int life) : Piece(w, x, y, initial), mLife(life), mAliveStep(0) {}
	virtual ~Critter() = default;
	virtual void Move(int direction);
	virtual void Move();

	int GetLife() const { return mLife; }
	int GetAliveStep() const { return mAliveStep; }
protected:
	int mLife;
	int mAliveStep;
};