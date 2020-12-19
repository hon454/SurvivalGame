#pragma once

class World;

class Piece
{
public:
	Piece(World& w, int x, int y, char initial) : mWorld(w), mX(x), mY(y), mInitial(initial) {}
	virtual ~Piece() = default;
	int GetX() const { return mX; }
	int GetY() const { return mY; }
	char GetInitial() const { return mInitial; }
protected:
	int mX, mY;
	char mInitial;
	World& mWorld;
};
