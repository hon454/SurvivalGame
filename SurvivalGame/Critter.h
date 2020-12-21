#pragma once

#include "Piece.h"

class Critter : public Piece
{
public:
	Critter(World& w, int x, int y, char initial, int life)
		: Piece(w, x, y, initial), mLife(life), mAliveStep(1), mIsActioned(false) {}
	virtual ~Critter() = default;
	
	virtual void Move(int direction) {}
	virtual void Move() {}

	void Alive()
	{
		--mLife;
		++mAliveStep;
		mIsActioned = false;
	}
	int GetLife() const { return mLife; }
	int GetAliveStep() const { return mAliveStep; }
	bool IsActioned() const { return mIsActioned; }
	Piece* GetObscured() const { return mObscured; }

protected:
	int mLife;
	int mAliveStep;
	bool mIsActioned;
	Piece* mObscured;
};